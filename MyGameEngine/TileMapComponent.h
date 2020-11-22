#include "SpriteComponent.h"
#include <string>

class TileMapComponent : public SpriteComponent {
public:
	TileMapComponent(class Actor* owner, int drawOrder = 100);

	// �θ�Ŭ������ Draw/SetTexture �����ε�
	void Draw(SDL_Renderer* renderer);
	void SetTexture(SDL_Texture* texture);

	void LoadCSV(const std::string& fileName);
private:
	SDL_Texture* mTexture;
	int mTexWidth;
	int mTexHeight;
};