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
		// 소유자의 스케일(배율)값으로 너비/높이를 스케일
		r.w = static_cast<int>(mOwner->GetScale());
		r.h = static_cast<int>(mOwner->GetScale());
		// Center the rectangle around the position of the owner
		// 소유자의 위치를 중심으로 사각형을 배치
		r.x = static_cast<int>(0);
		r.y = static_cast<int>(0);

		SDL_Rect scr_r;

		// 소유자의 스케일(배율)값으로 너비/높이를 스케일
		scr_r.w = static_cast<int>(mTexWidth/8);
		scr_r.h = static_cast<int>(mTexHeight/24);
		// 소유자의 위치를 중심으로 사각형을 배치
		scr_r.x = static_cast<int>(1*scr_r.w);
		scr_r.y = static_cast<int>(0*scr_r.h);

		// Draw (have to convert angle from radians to degrees, and clockwise to counter)
		// 스프라이트를 그린다.
		SDL_RenderCopy(
			renderer,	//그려질 렌더 타깃
			mTexture,	// 그릴 텍스쳐
			&scr_r,		// 그릴 텍스처의 일부 영역( 전체 영역이면 nullptr )
			&r			// 타깃에 그릴 사각형 영역
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