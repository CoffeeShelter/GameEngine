#include "BGSpriteComponent.h"
#include "Texture.h"
#include "Actor.h"

BGSpriteComponent::BGSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner)
	, mScrollSpeed(0.0f)
{}

void BGSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);
	for (auto& bg : mBGTextures) {
		// x 오프셋 값을 갱신
		bg.mOffset.x += mScrollSpeed * deltaTime;

		// 이 텍스처가 화면을 완전히 벗어나면 오프셋 값을
		// 마지막 배경 텍스처의 오른쪽 위치로 초기화한다.
		if (bg.mOffset.x < -mScreenSize.x) {
			bg.mOffset.x = (mBGTextures.size() - 1) * mScreenSize.x - 1;
		}
	}
}

void BGSpriteComponent::Draw(Shader* shader) {
	// 각 배경을 그린다.
	for (auto& bg : mBGTextures) {
		
	}
}

// 배경에 사용되는 텍스처 설정
void BGSpriteComponent::SetBGTextures(const std::vector<Texture*>& textures) {
	int count = 0;

	for (auto tex : textures) {
		BGTexture temp;
		temp.mTexture = tex;
		// 각 텍스처의 오프셋은 화면의 너비 * count다
		temp.mOffset.x = count * mScreenSize.x;
		temp.mOffset.y = 0;

		mBGTextures.emplace_back(temp);
		count++;
	}
}