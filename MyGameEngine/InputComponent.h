#include "MoveComponent.h"
#include <cstdint>

class InputComponent : public MoveComponent {
public:
	InputComponent(class Actor* owner);

	void ProcessInput(const uint8_t* keyState) override;

	//private 변수에 접근하기 위한 getter/setter 함수들
	//getter
	float GetMaxForwardSpeed() const { return mMaxForwardSpeed; }
	float GetMaxAngularSpeed() const { return mMaxAngularSpeed; }
	int GetForwardKey() const { return mForwardKey; }
	int GetBackKey() const { return mBackKey; }
	int GetClockwiseKey() const { return mClockwiseKey; }
	int GetCounterClockwiseKey() const { return mCounterClockwiseKey; }

	//setter
	float SetMaxForwardSpeed(float speed) {  mMaxForwardSpeed = speed; }
	float SetMaxAngularSpeed(float speed) {  mMaxAngularSpeed = speed; }
	int SetForwardKey(int key) {  mForwardKey = key; }
	int SetBackKey(int key) {  mBackKey = key; }
	int SetClockwiseKey(int key) {  mClockwiseKey = key; }
	int SetCounterClockwiseKey(int key) {  mCounterClockwiseKey = key; }
private:
	// 최대 전방 속도/최대 각 속도
	float mMaxForwardSpeed;
	float mMaxAngularSpeed;
	
	// 전진 및 후진을 위한 키
	int mForwardKey;
	int mBackKey;

	// 각 운동을 위한 키
	int mClockwiseKey;
	int mCounterClockwiseKey;
};