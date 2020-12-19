#include "NavComponent.h"
#include "Actor.h"

NavComponent::NavComponent(Actor* owner, int updateOrder)
	:MoveComponent(owner, updateOrder)
	,mNextNode(nullptr)
{}

// 특점 점을 향하고자 액터를 회전시킴
void NavComponent::TurnTo(const Vector2& pos)
{
	// 향하려는 점으로의 벡터를 구한다
	Vector2 dir = pos - mOwner->GetPosition();
	// 새 각도는 이 방향 벡터의 atan2다
	// (y값을 반전시킨다. +y는 화면 아래 방향이기 때문)
	float angle = Math::Atan2(-dir.y, dir.x);
	mOwner->SetRotation(angle);
}

void NavComponent::Update(float deltaTime)
{	
	if(mNextNode)
	{
		// 다음 지점에 도착했다면 이동할 새 지점을 얻고 해당 지점을 향해 회전한다
		
	}
}