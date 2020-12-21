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
		// x ������ ���� ����
		bg.mOffset.x += mScrollSpeed * deltaTime;

		// �� �ؽ�ó�� ȭ���� ������ ����� ������ ����
		// ������ ��� �ؽ�ó�� ������ ��ġ�� �ʱ�ȭ�Ѵ�.
		if (bg.mOffset.x < -mScreenSize.x) {
			bg.mOffset.x = (mBGTextures.size() - 1) * mScreenSize.x - 1;
		}
	}
}

void BGSpriteComponent::Draw(Shader* shader) {
	// �� ����� �׸���.
	for (auto& bg : mBGTextures) {
		
	}
}

// ��濡 ���Ǵ� �ؽ�ó ����
void BGSpriteComponent::SetBGTextures(const std::vector<Texture*>& textures) {
	int count = 0;

	for (auto tex : textures) {
		BGTexture temp;
		temp.mTexture = tex;
		// �� �ؽ�ó�� �������� ȭ���� �ʺ� * count��
		temp.mOffset.x = count * mScreenSize.x;
		temp.mOffset.y = 0;

		mBGTextures.emplace_back(temp);
		count++;
	}
}