#include "NavComponent.h"
#include "Tile.h"

NavComponent::NavComponent(Actor* owner, int updateOrder)
	:MoveComponent(owner, updateOrder)
	,mNextNode(nullptr)
{}

void NavComponent::Update(float deltaTime)
{
	if (mNextNode)
	{
		// ���� ������ �����ߴٸ� �̵��� �� ������ ��� �ش� ������ ���� ȸ���Ѵ�
		Vector2 diff = mOwner->GetPosition() - mNextNode->GetPosition();
		if (Math::NearZero(diff.Length(), 2.0f))
		{
			mNextNode = mNextNode->GetParent();
			TurnTo(mNextNode->GetPosition());
		}
	}

	MoveComponent::Update(deltaTime);
}

void NavComponent::StartPath(const Tile* start)
{
	mNextNode = start->GetParent();
	TurnTo(mNextNode->GetPosition());
}

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