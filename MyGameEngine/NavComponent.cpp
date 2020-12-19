#include "NavComponent.h"
#include "Actor.h"

NavComponent::NavComponent(Actor* owner, int updateOrder)
	:MoveComponent(owner, updateOrder)
	,mNextNode(nullptr)
{}

// Ư�� ���� ���ϰ��� ���͸� ȸ����Ŵ
void NavComponent::TurnTo(const Vector2& pos)
{
	// ���Ϸ��� �������� ���͸� ���Ѵ�
	Vector2 dir = pos - mOwner->GetPosition();
	// �� ������ �� ���� ������ atan2��
	// (y���� ������Ų��. +y�� ȭ�� �Ʒ� �����̱� ����)
	float angle = Math::Atan2(-dir.y, dir.x);
	mOwner->SetRotation(angle);
}

void NavComponent::Update(float deltaTime)
{	
	if(mNextNode)
	{
		// ���� ������ �����ߴٸ� �̵��� �� ������ ��� �ش� ������ ���� ȸ���Ѵ�
		
	}
}