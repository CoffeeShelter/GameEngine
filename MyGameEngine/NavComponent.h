#include "MoveComponent.h"
#include "Math.h"

class NavComponent : public MoveComponent
{
public:
	// 먼저 업데이트 하도록 업데이트순서 낮추기
	NavComponent(class Actor* owner, int updateOrder = 10);
	void Update(float deltaTime) override;
	void StartPath(const class Tile* start);
	void TurnTo(const Vector2& pos);
private:
	const class Tile* mNextNode;
};