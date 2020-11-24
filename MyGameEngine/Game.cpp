#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "BGSpriteComponent.h"
#include "TileMapComponent.h"
#include "Ship.h"

#include <algorithm>
#include <SDL_image.h>

Game::Game()
	:mWindow(nullptr)
	, mRenderer(nullptr)
	, mIsRunning(true)
	, mUpdatingActors(false)
	, mTicksCount(0)
{}

// 게임 초기화
bool Game::Initialize() {
	int sdlResult = 0;

	sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"MyGameEngine",	// 윈도우 제목
		100,	// 윈도우의 좌측 상단 x좌표
		100,	// 윈도우의 우측 상단 y좌표
		1024,	// 윈도우 너비
		768,	// 윈도우 높이
		0		// 플래그 (0은 어떠한 플래그도 설정되지 않음을 의미)
	);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

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
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
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
	// 이스케이프 키를 눌렀다면 루프 종료
	if (keyState[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	mUpdatingActors = true;
	for (auto actor : mActors) {
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
	SDL_SetRenderDrawColor(mRenderer, 220, 220, 220, 255);
	SDL_RenderClear(mRenderer);

	// 모든 스프라이트 컴포넌트를 그린다.
	for (auto sprite : mSprites) {
		sprite->Draw(mRenderer);
	}
	SDL_RenderPresent(mRenderer);
}

void Game::LoadData() {
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(512.0f, 384.0f));
	mShip->SetRotation(Math::PiOver2);
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

SDL_Texture* Game::GetTexture(const std::string& fileName) {
	SDL_Texture* tex = nullptr;

	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end()) {
		tex = iter->second;
	}
	else {
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf) {
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex) {
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
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