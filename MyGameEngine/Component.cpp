#include "Component.h"
#include "Actor.h"

// ������
Component::Component(Actor* owner, int updateOrder)
	:mOwner(owner)
	, mUpdateOrder(updateOrder)
{
	// ������ ������Ʈ ���Ϳ� �߰�
	mOwner->AddComponent(this);
}

// �Ҹ���
Component::~Component(){
	mOwner->RemoveComponent(this);
}

// ������Ʈ
void Component::Update(float deltaTime){}