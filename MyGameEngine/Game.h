#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include <unordered_map>

class Game {
public:
	Game();
	// 게임 초기화
	bool Initialize();
	// 게임이 끝나기 전까지 게임 루프 실행
	void RunLoop();
	// 게임 종료
	void Shutdown();

	// 액터 추가/제거
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	// 스프라이트 추가/제거
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);

private:
	// 게임 루프를 위한 헬퍼 함수
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// SDL이 생성한 윈도우
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;

	// 게임이 계속 실행돼야 하는지를 판단
	bool mIsRunning;
	// 현재 액터가 갱신중인지 판단
	bool mUpdatingActors;

	// 활성화된 액터
	std::vector<class Actor*> mActors;
	// 대기 중인 액터
	std::vector<class Actor*> mPendingActors;
	// 그려질 모든 스프라이트
	std::vector<class SpriteComponent*> mSprites;

	// Map of textures loaded
	std::unordered_map<std::string, SDL_Texture*> mTextures;
};