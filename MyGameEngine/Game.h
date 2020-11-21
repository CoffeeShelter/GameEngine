#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include <unordered_map>

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

	SDL_Texture* GetTexture(const std::string& fileName);

private:
	// ���� ������ ���� ���� �Լ�
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// SDL�� ������ ������
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;

	// ������ ��� ����ž� �ϴ����� �Ǵ�
	bool mIsRunning;
	// ���� ���Ͱ� ���������� �Ǵ�
	bool mUpdatingActors;

	// Ȱ��ȭ�� ����
	std::vector<class Actor*> mActors;
	// ��� ���� ����
	std::vector<class Actor*> mPendingActors;
	// �׷��� ��� ��������Ʈ
	std::vector<class SpriteComponent*> mSprites;

	// Map of textures loaded
	std::unordered_map<std::string, SDL_Texture*> mTextures;
};