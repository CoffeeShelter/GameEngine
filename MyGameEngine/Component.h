#pragma once
#include <cstdint>

class Component {
public:
	// ������
	// (������Ʈ �������� �������� ������Ʈ�� �� ���� ���ŵȴ�)
	Component(class Actor* owner, int updateOrder = 100);
	// �Ҹ���
	virtual ~Component();
	// ��Ÿ �ð����� �� ������Ʈ�� ������Ʈ
	virtual void Update(float deltaTime);
	virtual void ProcessInput(const uint8_t* keyState) {}

	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	// ������ ����
	class Actor* mOwner;
	// ������Ʈ ������Ʈ ����
	int mUpdateOrder;
};