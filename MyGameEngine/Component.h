#pragma once
#include <cstdint>

class Component {
public:
	// 생성자
	// (업데이트 순서값이 작을수록 컴포넌트는 더 빨리 갱신된다)
	Component(class Actor* owner, int updateOrder = 100);
	// 소멸자
	virtual ~Component();
	// 델타 시간으로 이 컴포넌트를 업데이트
	virtual void Update(float deltaTime);
	// Process input for this component
	virtual void ProcessInput(const uint8_t* keyState) {}
	// Called when world transform changes
	virtual void OnUpdateWorldTransform() {}

	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	// 소유자 액터
	class Actor* mOwner;
	// 컴포넌트 업데이트 순서
	int mUpdateOrder;
};