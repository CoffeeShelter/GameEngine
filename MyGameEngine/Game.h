#pragma once
#include "SDL/SDL.h"
#include <vector>
#include <string>
#include <unordered_map>
#include "Math.h"

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

	class Texture* GetTexture(const std::string& fileName);

	// 특정 게임
	// ( 디펜스 )
	class Grid* GetGrid() { return mGrid; }
	std::vector<class Enemy*>& GetEnemies() { return mEnemies; }
	class Enemy* GetNearestEnemy(const Vector2& pos);

	// ( 아스트로이드 )
	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }

private:
	// 게임 루프를 위한 헬퍼 함수
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	bool LoadShaders();
	void CreateSpriteVerts();

	// 활성화된 액터
	std::vector<class Actor*> mActors;
	// 대기 중인 액터
	std::vector<class Actor*> mPendingActors;
	// 그려질 모든 스프라이트
	std::vector<class SpriteComponent*> mSprites;

	// Map of textures loaded
	std::unordered_map<std::string, class Texture*> mTextures;

	// 스프라이트 쉐이더
	class Shader* mSpriteShader;
	// Sprite vertex array
	class VertexArray* mSpriteVerts;

	// SDL이 생성한 윈도우
	SDL_Window* mWindow;
	Uint32 mTicksCount;
	SDL_GLContext mContext;

	// 게임이 계속 실행돼야 하는지를 판단
	bool mIsRunning;
	// 현재 액터가 갱신중인지 판단
	bool mUpdatingActors;

	// 특정 게임
	std::vector<class Enemy*> mEnemies;
	class Grid* mGrid;
	float mNextEnemy;

	// ( 아스트로이드 )
	class Ship* mShip; // Player's Ship
	std::vector<class Asteroid*> mAsteroids;
};