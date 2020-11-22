#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	:mState(EActive)
	, mPosition(Vector2::Zero)
	, mScale(1.0f)
	, mRotation(0.0f)
	, mGame(game)
{
	mGame->AddActor(this);
}

Actor::~Actor() {
	mGame->RemoveActor(this);

	while (!mComponents.empty()) {
		delete mComponents.back();
	}
}

// Game에서 호출하는 Update 함수 (가상 함수 아님)
void Actor::Update(float deltaTime) {
	if (mState == EActive) {
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

// 액터에 부착된 모든 컴포넌트를 업데이트 (가상 함수 아님)
void Actor::UpdateComponents(float deltaTime) {
	for (auto comp : mComponents) {
		comp->Update(deltaTime);
	}
}

// 특정 액터에 특화된 업데이트 코드 (오버라이딩 가능)
void Actor::UpdateActor(float deltaTime) {

}

// 컴포넌트 추가
void Actor::AddComponent(class Component* component) {
	// 정렬된 벡터에서 삽입해야 할 위치를 찾는다.
	// (자신보다 순서값이 큰 최초 요소)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); ++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) {
			break;
		}
	}
	mComponents.insert(iter, component);
}

// 컴포넌트 제거
void Actor::RemoveComponent(class Component* component) {
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end()) {
		mComponents.erase(iter);
	}
}