#include "Component.h"
#include "Actor.h"

// 생성자
Component::Component(Actor* owner, int updateOrder)
	:mOwner(owner)
	, mUpdateOrder(updateOrder)
{
	// 액터의 컴포넌트 벡터에 추가
	mOwner->AddComponent(this);
}

// 소멸자
Component::~Component(){
	mOwner->RemoveComponent(this);
}

// 업데이트
void Component::Update(float deltaTime){}