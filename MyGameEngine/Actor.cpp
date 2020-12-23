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

// Game���� ȣ���ϴ� Update �Լ� (���� �Լ� �ƴ�)
void Actor::Update(float deltaTime) {
	if (mState == EActive) {
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		ComputeWorldTransform();
	}
}

// Game���� ȣ���ϴ� ProcessInput �Լ� (������ ���� ����)
void Actor::ProcessInput(const uint8_t* keyState) {
	if (mState == EActive) {
		for (auto comp : mComponents) {
			comp->ProcessInput(keyState);
		}
		ActorInput(keyState);
	}
}

// Ư�� ���͸� ���� �Է� �ڵ� (������ ��)
void Actor::ActorInput(const uint8_t* keyState) {

}

// ���Ϳ� ������ ��� ������Ʈ�� ������Ʈ (���� �Լ� �ƴ�)
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
		// ������, ȸ��, �̵� ��ļ����� �����ؼ� ���� ��ȯ ��� ����
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(
			Vector3(mPosition.x, mPosition.y, 0.0f));
	}

	// ������Ʈ�� ���� ��ȯ�� ���ŵƴٰ� ����
	for (auto comp : mComponents)
	{
		comp->OnUpdateWorldTransform();
	}

}


// Ư�� ���Ϳ� Ưȭ�� ������Ʈ �ڵ� (�������̵� ����)
void Actor::UpdateActor(float deltaTime) {

}

// ������Ʈ �߰�
void Actor::AddComponent(class Component* component) {
	// ���ĵ� ���Ϳ��� �����ؾ� �� ��ġ�� ã�´�.
	// (�ڽź��� �������� ū ���� ���)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); ++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) {
			break;
		}
	}
	mComponents.insert(iter, component);
}

// ������Ʈ ����
void Actor::RemoveComponent(class Component* component) {
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end()) {
		mComponents.erase(iter);
	}
}