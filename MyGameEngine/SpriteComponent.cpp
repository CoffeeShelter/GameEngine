#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Shader.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
	, mTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mTexWidth(0)
	, mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	mOwner->GetGame()->RemoveSprite(this);
}

// 그리기
void SpriteComponent::Draw(Shader* shader) {
	if (mTexture) {
		// Scale the quad by the width/height of texture
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f
		);

		Matrix4 world = scaleMat * mOwner->GetWorldTransform();
		// 세계 변환 행렬을 설정
		shader->SetMatrixUniform("uWorldTransform", world);
		// 사각형을 그린다
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

// 텍스처 설정
void SpriteComponent::SetTexture(SDL_Texture* texture) {
	mTexture = texture;
	// 텍스처 너비/높이 설정
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}