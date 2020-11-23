#include "TileMapComponent.h"
#include "Math.h"
#include "Actor.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define _CRT_SECURE_NO_WARNINGS

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner)
	, mTexture(nullptr)
	, mTexHeight(0)
	, mTexWidth(0)
{
}

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
		r.x = static_cast<int>(0 * (r.w / 8) * 1.5);
		r.y = static_cast<int>(0 * (r.h / 24) * 1.5);

		SDL_Rect scr_r;

		// 그리길 원하는 텍스처 내부 너비/높이
		scr_r.w = static_cast<int>(mTexWidth / 8);
		scr_r.h = static_cast<int>(mTexHeight / 24);
		// 그리길 원하는 텍스처 내부 좌표 x/y
		scr_r.x = static_cast<int>(1 * scr_r.w);
		scr_r.y = static_cast<int>(0 * scr_r.h);

		int countCol = 0;
		int countLine = 0;


		auto mapsiter = maps.begin();
		for (; mapsiter != maps.end();++mapsiter) {
			countCol = 0;
			countLine = 0;
			
			auto mapIter = (*mapsiter).begin();
			for (; mapIter != (*mapsiter).end();++mapIter) {

				auto dataIter = (*mapIter).begin();
				for (; dataIter != (*mapIter).end();++dataIter) {
					if ((*dataIter) != -1) {
						// Center the rectangle around the position of the owner
						// 소유자의 위치를 중심으로 사각형을 배치
						r.x = static_cast<int>(countCol * r.w);
						r.y = static_cast<int>(countLine * r.h);

						// 그리길 원하는 텍스처 내부 좌표 x/y
						scr_r.x = static_cast<int>(((*dataIter) % 8 ) * scr_r.w);
						scr_r.y = static_cast<int>(((*dataIter) / 8 ) * scr_r.h);

						// Draw (have to convert angle from radians to degrees, and clockwise to counter)
						// 스프라이트를 그린다.
						SDL_RenderCopy(
							renderer,	//그려질 렌더 타깃
							mTexture,	// 그릴 텍스쳐
							&scr_r,		// 그릴 텍스처의 일부 영역( 전체 영역이면 nullptr )
							&r			// 타깃에 그릴 사각형 영역
						);
					}
					countCol += 1;
					if (countCol >= static_cast<int>((*mapIter).size())) countCol = 0;
				}
				countLine += 1;
				if (countLine >= static_cast<int>((*mapsiter).size())) countLine = 0;
			}
		}
	}
}

void TileMapComponent::SetTexture(SDL_Texture* texture) {
	mTexture = texture;

	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}

void TileMapComponent::LoadCSV(const std::string& fileName) {
	std::vector<std::vector<int>> map;
	std::vector<int> data;
	std::ifstream csvFile;

	int value = 0;

	csvFile.open(fileName);

	int count = 0;
	std::string buf;

	while (csvFile.peek()!=EOF) {
		std::getline(csvFile, buf);
		std::stringstream s(buf);

		while (std::getline(s, buf, ',')) {
			char cBuf[5] = { '\0' };
			strcpy_s(cBuf, buf.c_str());
			value = atoi(cBuf);
			data.push_back(value);
		}

		map.push_back(data);
		data.clear();
	}
	maps.push_back(map);

	csvFile.close();
}

//std::cout << "(" << value % 7 << "," << value / 7 << ") ";