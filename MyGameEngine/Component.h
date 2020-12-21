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
	// Process input for this component
	virtual void ProcessInput(const uint8_t* keyState) {}
	// Called when world transform changes
	virtual void OnUpdateWorldTransform() {}

	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	// ������ ����
	class Actor* mOwner;
	// ������Ʈ ������Ʈ ����
	int mUpdateOrder;
};