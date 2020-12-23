#include "CircleComponent.h"
#include "Actor.h"

CircleComponent::CircleComponent(Actor* owner)
	:Component(owner)
	,mRadius(0.0f)
{}

const Vector3& CircleComponent::GetCenter() const {
	return mOwner->GetPosition();
}

float CircleComponent::GetRadius() const {
	return mOwner->GetScale() * mRadius;
}

bool Intersect(const CircleComponent& a, const CircleComponent& b) {
	// 거리 제곱값을 계산
	Vector3 diff = a.GetCenter() - b.GetCenter();
	float distSq = diff.LengthSq();

	// 반지름 합의 제곱값을 계산
	float radiiSq = a.GetRadius() + b.GetRadius();
	radiiSq *= radiiSq;

	return distSq <= radiiSq;
}