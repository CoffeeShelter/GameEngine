#include "TileMapComponent.h"
#include "Math.h"
#include "Actor.h"
#include <iostream>
#include <fstream>
#include <string>

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner)
	, mTexture(nullptr)
	,mTexHeight(0)
	,mTexWidth(0)
{}

void TileMapComponent::Draw(SDL_Renderer* renderer) {
	if (mTexture)
	{
		SDL_Rect r;
		// Scale the width/height by owner's scale
		// �������� ������(����)������ �ʺ�/���̸� ������
		r.w = static_cast<int>(mOwner->GetScale());
		r.h = static_cast<int>(mOwner->GetScale());
		// Center the rectangle around the position of the owner
		// �������� ��ġ�� �߽����� �簢���� ��ġ
		r.x = static_cast<int>(0);
		r.y = static_cast<int>(0);

		SDL_Rect scr_r;

		// �������� ������(����)������ �ʺ�/���̸� ������
		scr_r.w = static_cast<int>(mTexWidth/8);
		scr_r.h = static_cast<int>(mTexHeight/24);
		// �������� ��ġ�� �߽����� �簢���� ��ġ
		scr_r.x = static_cast<int>(1*scr_r.w);
		scr_r.y = static_cast<int>(0*scr_r.h);

		// Draw (have to convert angle from radians to degrees, and clockwise to counter)
		// ��������Ʈ�� �׸���.
		SDL_RenderCopy(
			renderer,	//�׷��� ���� Ÿ��
			mTexture,	// �׸� �ؽ���
			&scr_r,		// �׸� �ؽ�ó�� �Ϻ� ����( ��ü �����̸� nullptr )
			&r			// Ÿ�꿡 �׸� �簢�� ����
		);
	}
}

void TileMapComponent::SetTexture(SDL_Texture* texture) {
	mTexture = texture;

	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}

void TileMapComponent::LoadCSV(const std::string& fileName) {
	std::ifstream csvFile;
	csvFile.open(fileName);

	csvFile.close();
}