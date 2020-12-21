#pragma once
#include "SDL/SDL.h"
#include <vector>
#include <string>
#include <unordered_map>
#include "Math.h"

class Game {
public:
	Game();
	// ���� �ʱ�ȭ
	bool Initialize();
	// ������ ������ ������ ���� ���� ����
	void RunLoop();
	// ���� ����
	void Shutdown();

	// ���� �߰�/����
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	// ��������Ʈ �߰�/����
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	class Texture* GetTexture(const std::string& fileName);

	// Ư�� ����
	// ( ���潺 )
	class Grid* GetGrid() { return mGrid; }
	std::vector<class Enemy*>& GetEnemies() { return mEnemies; }
	class Enemy* GetNearestEnemy(const Vector2& pos);

	// ( �ƽ�Ʈ���̵� )
	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }

private:
	// ���� ������ ���� ���� �Լ�
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	bool LoadShaders();
	void CreateSpriteVerts();

	// Ȱ��ȭ�� ����
	std::vector<class Actor*> mActors;
	// ��� ���� ����
	std::vector<class Actor*> mPendingActors;
	// �׷��� ��� ��������Ʈ
	std::vector<class SpriteComponent*> mSprites;

	// Map of textures loaded
	std::unordered_map<std::string, class Texture*> mTextures;

	// ��������Ʈ ���̴�
	class Shader* mSpriteShader;
	// Sprite vertex array
	class VertexArray* mSpriteVerts;

	// SDL�� ������ ������
	SDL_Window* mWindow;
	Uint32 mTicksCount;
	SDL_GLContext mContext;

	// ������ ��� ����ž� �ϴ����� �Ǵ�
	bool mIsRunning;
	// ���� ���Ͱ� ���������� �Ǵ�
	bool mUpdatingActors;

	// Ư�� ����
	std::vector<class Enemy*> mEnemies;
	class Grid* mGrid;
	float mNextEnemy;

	// ( �ƽ�Ʈ���̵� )
	class Ship* mShip; // Player's Ship
	std::vector<class Asteroid*> mAsteroids;
};