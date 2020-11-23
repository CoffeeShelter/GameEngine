#include "MoveComponent.h"
#include <cstdint>

class InputComponent : public MoveComponent {
public:
	InputComponent(class Actor* owner);

	void ProcessInput(const uint8_t* keyState) override;

	//private ������ �����ϱ� ���� getter/setter �Լ���
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
	// �ִ� ���� �ӵ�/�ִ� �� �ӵ�
	float mMaxForwardSpeed;
	float mMaxAngularSpeed;
	
	// ���� �� ������ ���� Ű
	int mForwardKey;
	int mBackKey;

	// �� ��� ���� Ű
	int mClockwiseKey;
	int mCounterClockwiseKey;
};