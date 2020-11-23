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
		// �������� ������(����)������ �ʺ�/���̸� ������
		r.w = static_cast<int>(mOwner->GetScale());
		r.h = static_cast<int>(mOwner->GetScale());
		// Center the rectangle around the position of the owner
		// �������� ��ġ�� �߽����� �簢���� ��ġ
		r.x = static_cast<int>(0 * (r.w / 8) * 1.5);
		r.y = static_cast<int>(0 * (r.h / 24) * 1.5);

		SDL_Rect scr_r;

		// �׸��� ���ϴ� �ؽ�ó ���� �ʺ�/����
		scr_r.w = static_cast<int>(mTexWidth / 8);
		scr_r.h = static_cast<int>(mTexHeight / 24);
		// �׸��� ���ϴ� �ؽ�ó ���� ��ǥ x/y
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
						// �������� ��ġ�� �߽����� �簢���� ��ġ
						r.x = static_cast<int>(countCol * r.w);
						r.y = static_cast<int>(countLine * r.h);

						// �׸��� ���ϴ� �ؽ�ó ���� ��ǥ x/y
						scr_r.x = static_cast<int>(((*dataIter) % 8 ) * scr_r.w);
						scr_r.y = static_cast<int>(((*dataIter) / 8 ) * scr_r.h);

						// Draw (have to convert angle from radians to degrees, and clockwise to counter)
						// ��������Ʈ�� �׸���.
						SDL_RenderCopy(
							renderer,	//�׷��� ���� Ÿ��
							mTexture,	// �׸� �ؽ���
							&scr_r,		// �׸� �ؽ�ó�� �Ϻ� ����( ��ü �����̸� nullptr )
							&r			// Ÿ�꿡 �׸� �簢�� ����
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