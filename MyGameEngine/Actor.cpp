#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	:mState(EActive)
	, mPosition(Vector3::Zero)
	, mScale(1.0f)
	, mRotation(Quaternion::Identity)
	, mGame(game)
	,mRecomputeWorldTransform(true)
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
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		ComputeWorldTransform();
	}
}

// Game에서 호출하는 ProcessInput 함수 (재정의 되지 않음)
void Actor::ProcessInput(const uint8_t* keyState) {
	if (mState == EActive) {
		for (auto comp : mComponents) {
			comp->ProcessInput(keyState);
		}
		ActorInput(keyState);
	}
}

// 특정 액터를 위한 입력 코드 (재정의 됨)
void Actor::ActorInput(const uint8_t* keyState) {

}

// 액터에 부착된 모든 컴포넌트를 업데이트 (가상 함수 아님)
void Actor::UpdateComponents(float deltaTime) {
	for (auto comp : mComponents) {
		comp->Update(deltaTime);
	}
}

void Actor::ComputeWorldTransform()
{
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;
		// 스케일, 회전, 이동 행렬순으로 결합해서 세계 변환 행렬 구함
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(
			Vector3(mPosition.x, mPosition.y, 0.0f));
	}

	// 컴포넌트에 세계 변환이 갱신됐다고 통지
	for (auto comp : mComponents)
	{
		comp->OnUpdateWorldTransform();
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