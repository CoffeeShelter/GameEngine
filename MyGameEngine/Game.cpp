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

// Ư�� ����
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

// ���� �ʱ�ȭ
bool Game::Initialize() {
	int sdlResult = 0;

	sdlResult = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Set OpenGL attributes
	// �ھ� OpenGL �������� ���
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// 3.3 �������� ����
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// RGBA ä�θ��� 8��Ʈ ũ���� ���� ���� ��û
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// ���� ���۸� Ȱ��ȭ
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// OpenGL�� �ϵ���� ������ ����ϵ��� ���� (GPU ���� ����� ������ ����)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow(
		"MyGameEngine",	// ������ ����
		100,	// �������� ���� ��� x��ǥ
		100,	// �������� ���� ��� y��ǥ
		1024,	// ������ �ʺ�
		768,	// ������ ����
		SDL_WINDOW_OPENGL		// �÷��� (0�� ��� �÷��׵� �������� ������ �ǹ�)
	);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mContext = SDL_GL_CreateContext(mWindow);

	// GLEW �ʱ�ȭ
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}
	// �Ϻ� �÷������� GLEW�� ���� �ڵ带 ��������.
	// �׷��Ƿ� �������� ��������
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

// ������ ������ ������ ���� ���� ����
void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

// ���� ����
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
	// ť�� ������ �̺�Ʈ�� ���� �ִ� ����
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// Ű������ ���� ���
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
	// ������ ������ ���ķ� 16ms�� ����� ������ ���
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// ��Ÿ �ð��� ���� ������ ƽ���� ���� ������ ƽ���� ����.
	// (���� �� ������ ��ȯ)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// ���� �������� ���� ƽ���� ����
	
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	// ��� ���͸� ����
	mUpdatingActors = true;
	for (auto actor : mActors) {
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// ��� ���� ���͸� mActors�� �̵�
	for (auto pending : mPendingActors) {
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// ���� ���͸� �ӽ� ���Ϳ� �߰�
	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) {
			deadActors.emplace_back(actor);
		}
	}

	// ���� ���� ����(mActors���� �߷��� ���͵�)
	for (auto actor : deadActors) {
		delete actor;
	}
}

void Game::GenerateOutput() {
	// ������ ȸ������ ����
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	// ���� ���� �ʱ�ȭ
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw all sprite components
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ��� ��������Ʈ ������Ʈ�� �׸���.
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

// ���� �߰�
void Game::AddActor(Actor* actor) {
	// ���͸� ���� ���̶�� ��� ���Ϳ� ���͸� �߰�
	if (mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	}
	else {
		mActors.emplace_back(actor);
	}
}

// ���� ����
void Game::RemoveActor(Actor* actor) {
	// ��� ���� �˻�
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()) {
		// �� �ڿ�Ҷ� ��ġ�� �ٲ� �� �ǵ� ��� ����
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}
	// Ȱ�� ���� �˻�
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		// �� �ڿ�Ҷ� ��ġ�� �ٲ� �� �ǵ� ��� ����
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

// ��������Ʈ �߰�
void Game::AddSprite(class SpriteComponent* sprite) {
	// ���ĵ� ���Ϳ��� �����ؾ� �� ��ġ�� ã�´�.
	// (�ڽź��� �׸��� �������� ū ���� ���)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) {
			break;
		}
	}
	mSprites.insert(iter, sprite);
}

// ��������Ʈ ����
void Game::RemoveSprite(class SpriteComponent* sprite) {
	// ����: (��ġ�� �ٲٸ� �׸��� ������ ��Ų��)
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

// Ư�� ����
// ( ���潺 )
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

// ( �ƽ�Ʈ���̵� )
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