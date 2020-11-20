#include "Game.h"

Game::Game()
	:mWindow(nullptr),
	mIsRunning(true),
	mTicksCount(0)
{}

// ���� �ʱ�ȭ
bool Game::Initialize() {
	int sdlResult = 0;

	sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"MyGameEngine",	// ������ ����
		100,	// �������� ���� ��� x��ǥ
		100,	// �������� ���� ��� y��ǥ
		1024,	// ������ �ʺ�
		768,	// ������ ����
		0		// �÷��� (0�� ��� �÷��׵� �������� ������ �ǹ�)
	);

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
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// �̽������� Ű�� �����ٸ� ���� ����
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}
}

void Game::UpdateGame() {
	// ������ ������ ���ķ� 16ms�� ����� ������ ���
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// ��Ÿ �ð��� ���� ������ ƽ���� ���� ������ ƽ���� ����.
	// (���� �� ������ ��ȯ)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// ���� �������� ���� ƽ���� ����
	mTicksCount = SDL_GetTicks();

	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
}

void Game::GenerateOutput() {

}