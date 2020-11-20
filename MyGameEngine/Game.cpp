#include "Game.h"

Game::Game()
	:mWindow(nullptr),
	mIsRunning(true),
	mTicksCount(0)
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
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// 이스케이프 키를 눌렀다면 루프 종료
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}
}

void Game::UpdateGame() {
	// 마지막 프레임 이후로 16ms가 경과할 때까지 대기
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// 델타 시간은 현재 프레임 틱값과 이전 프레임 틱값의 차다.
	// (차를 초 단위로 변환)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// 다음 프레임을 위해 틱값을 갱신
	mTicksCount = SDL_GetTicks();

	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
}

void Game::GenerateOutput() {

}