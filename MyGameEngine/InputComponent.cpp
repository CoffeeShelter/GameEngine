#include "InputComponent.h"
#include "Actor.h"

InputComponent::InputComponent(Actor* owner)
	:MoveComponent(owner)
	,mMaxAngularSpeed(0.0f)
	,mMaxForwardSpeed(0.0f)
	,mClockwiseKey(0)
	,mCounterClockwiseKey(0)
	,mForwardKey(0)
	,mBackKey(0)
{}

void InputComponent::ProcessInput(const uint8_t* keyState) {
	// MoveComponent �� ���� ���� �ӵ� ���
	float forwardSpeed = 0.0f;
	if (keyState[mForwardKey]) {
		forwardSpeed += mMaxForwardSpeed;
	}
	if (keyState[mBackKey]) {
		forwardSpeed -= mMaxForwardSpeed;
	}
	SetForwardSpeed(forwardSpeed);

	// MoveComponent �� ���� �� �ӵ� ���
	float angularSpeed = 0.0f;
	if (keyState[mClockwiseKey]) {
		angularSpeed += mMaxAngularSpeed;
	}
	if (keyState[mCounterClockwiseKey]) {
		angularSpeed -= mMaxAngularSpeed;
	}
	SetAngularSpeed(angularSpeed);
}