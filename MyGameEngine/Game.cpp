#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "BGSpriteComponent.h"
#include "TileMapComponent.h"
#include "AIComponent.h"
#include "AIState.h"
#include "Shader.h"
#include "VertexArray.h"
#include <GL/glew.h>

// 특정 게임
#include "Ship.h"
#include "Asteroid.h"

#include <algorithm>
#include "SDL/SDL_image.h"

Game::Game()
	:mWindow(nullptr)
	, mIsRunning(true)
	, mUpdatingActors(false)
	, mTicksCount(0)
{}

// 게임 초기화
bool Game::Initialize() {
	int sdlResult = 0;

	sdlResult = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Set OpenGL attributes
	// 코어 OpenGL 프로파일 사용
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// 3.3 버전으로 지정
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// RGBA 채널마다 8비트 크기인 색상 버퍼 요청
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// 더블 버퍼링 활성화
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// OpenGL이 하드웨어 가속을 사용하도록 강제 (GPU 에서 수행될 것임을 뜻함)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow(
		"MyGameEngine",	// 윈도우 제목
		100,	// 윈도우의 좌측 상단 x좌표
		100,	// 윈도우의 우측 상단 y좌표
		1024,	// 윈도우 너비
		768,	// 윈도우 높이
		SDL_WINDOW_OPENGL		// 플래그 (0은 어떠한 플래그도 설정되지 않음을 의미)
	);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mContext = SDL_GL_CreateContext(mWindow);

	// GLEW 초기화
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}
	// 일부 플랫폼에서 GLEW은 에러 코드를 내보낸다.
	// 그러므로 에러값을 제거하자
	glGetError();

	// Make sure we can create/compile shaders
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}
	
	// Create quad for drawing sprites
	CreateSpriteVerts();

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

// 게임이 끝나기 전까지 게임 루프 실행
void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

// 게임 종료
void Game::Shutdown() {
	UnloadData();
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::ProcessInput() {
	SDL_Event event;
	// 큐에 여전히 이벤트가 남아 있는 동안
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// 키보드의 상태 얻기
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
}

void Game::UpdateGame() {
	// 마지막 프레임 이후로 16ms가 경과할 때까지 대기
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// 델타 시간은 현재 프레임 틱값과 이전 프레임 틱값의 차다.
	// (차를 초 단위로 변환)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// 다음 프레임을 위해 틱값을 갱신
	
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	// 모든 액터를 갱신
	mUpdatingActors = true;
	for (auto actor : mActors) {
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// 대기 중인 액터를 mActors로 이동
	for (auto pending : mPendingActors) {
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// 죽은 액터를 임시 벡터에 추가
	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) {
			deadActors.emplace_back(actor);
		}
	}

	// 죽은 액터 제거(mActors에서 추려낸 액터들)
	for (auto actor : deadActors) {
		delete actor;
	}
}

void Game::GenerateOutput() {
	// 색상을 회색으로 설정
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	// 색상 버퍼 초기화
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw all sprite components
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 모든 스프라이트 컴포넌트를 그린다.
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();
	for (auto sprite : mSprites) {
		sprite->Draw(mSpriteShader);
	}

	// Swap the buffers
	SDL_GL_SwapWindow(mWindow);
}

bool Game::LoadShaders()
{
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Transform.vert", "Shaders/Basic.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();

	// Set the view-projection matrix
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.0f, 768.0f);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	return true;
}

void Game::LoadData() {
	// Create player's ship
	mShip = new Ship(this);
	mShip->SetRotation(Math::PiOver2);

	// Create asteroids
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
}

void Game::UnloadData() {
	while (!mActors.empty()) {
		delete mActors.back();
	}

	for (auto i : mTextures) {
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

void Game::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, // top left
		 0.5f,  0.5f, 0.f, 1.f, 0.f, // top right
		 0.5f, -0.5f, 0.f, 1.f, 1.f, // bottom right
		-0.5f, -0.5f, 0.f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

SDL_Texture* Game::GetTexture(const std::string& fileName) {
	SDL_Texture* tex = nullptr;

	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end()) {
		tex = iter->second;
	}
	else {
		
	}

	return tex;
}

// 액터 추가
void Game::AddActor(Actor* actor) {
	// 액터를 갱신 중이라면 대기 벡터에 액터를 추가
	if (mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	}
	else {
		mActors.emplace_back(actor);
	}
}

// 액터 제거
void Game::RemoveActor(Actor* actor) {
	// 대기 벡터 검사
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()) {
		// 맨 뒤요소랑 위치를 바꾼 후 맨뒤 요소 제거
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}
	// 활성 벡터 검사
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		// 맨 뒤요소랑 위치를 바꾼 후 맨뒤 요소 제거
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

// 스프라이트 추가
void Game::AddSprite(class SpriteComponent* sprite) {
	// 정렬된 벡터에서 삽입해야 할 위치를 찾는다.
	// (자신보다 그리기 순서값이 큰 최초 요소)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) {
			break;
		}
	}
	mSprites.insert(iter, sprite);
}

// 스프라이트 제거
void Game::RemoveSprite(class SpriteComponent* sprite) {
	// 주의: (위치를 바꾸면 그리기 순서가 엉킨다)
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

// 특정 게임
// ( 디펜스 )
Enemy* Game::GetNearestEnemy(const Vector2& pos)
{
	
	Enemy* best = nullptr;
	/*
	if (mEnemies.size() > 0)
	{
		best = mEnemies[0];
		// Save the distance squared of first enemy, and test if others are closer
		float bestDistSq = (pos - mEnemies[0]->GetPosition()).LengthSq();
		for (size_t i = 1; i < mEnemies.size(); i++)
		{
			float newDistSq = (pos - mEnemies[i]->GetPosition()).LengthSq();
			if (newDistSq < bestDistSq)
			{
				bestDistSq = newDistSq;
				best = mEnemies[i];
			}
		}
	}
	*/
	return best;
}

// ( 아스트로이드 )
void Game::AddAsteroid(Asteroid* ast)
{
	mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(mAsteroids.begin(),
		mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
	}
}