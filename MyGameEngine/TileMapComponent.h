#include "SpriteComponent.h"
#include <string>
#include <vector>

class TileMapComponent : public SpriteComponent {
public:
	TileMapComponent(class Actor* owner, int drawOrder = 100);

	// �θ�Ŭ������ Draw/SetTexture �����ε�
	void Draw(SDL_Renderer* renderer);
	void SetTexture(SDL_Texture* texture);

	void LoadCSV(const std::string& fileNamej);
private:
	SDL_Texture* mTexture;
	int mTexWidth;
	int mTexHeight;

	std::vector<std::vector<std::vector<int>>> maps;
};