#pragma once

#include "SpriteComponent.h"
#include <vector>
#include "Math.h"

class BGSpriteComponent : public SpriteComponent {
public:
	// 그리기 순서를 낮은 값으로 설정 (배경이므로)
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);
	// 부모로부터 오버라이딩한 Update/draw 함수
	void Update(float deltaTime) override;
	void Draw(Shader* shader) override;
	// 배경에 사용되는 텍스처 설정
	void SetBGTextures(const std::vector<class Texture*>& textures);
	
	// 화면 크기를 얻거나 스크롤 속도값을 설정 및 얻는 함수
	// Getter/Setter
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed() const { return mScrollSpeed; }
private:
	// 각 배경 이미지와 배경 이미지의 오프셋 값을 캡슐화한 구조체
	struct BGTexture {
		class Texture* mTexture;
		Vector2 mOffset;
	};
	std::vector<BGTexture> mBGTextures;
	Vector2 mScreenSize;
	float mScrollSpeed;
};