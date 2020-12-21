#pragma once
#include "Math.h"
#include <vector>

class Actor {
public:
	// ������ ���¸� �����ϴµ� ���ȴ�.
	enum State {
		EActive,
		EPaused,
		EDead
	};

	// ������/�Ҹ���
	Actor(class Game* game);
	virtual ~Actor();

	// Game���� ȣ���ϴ� Update �Լ� (���� �Լ� �ƴ�)
	void Update(float deltaTime);
	// ���Ϳ� ������ ��� ������Ʈ�� ������Ʈ (���� �Լ� �ƴ�)
	void UpdateComponents(float deltaTime);
	// Ư�� ���Ϳ� Ưȭ�� ������Ʈ �ڵ� (�������̵� ����)
	virtual void UpdateActor(float deltaTime);

	// Game�� ȣ���ϴ� ProcessInput �Լ� (������ ���� ����)
	void ProcessInput(const uint8_t* keyState);
	// Ư�� ���͸� ���� �Է� �ڵ� (�����ǵ�)
	virtual void ActorInput(const uint8_t* keyState);

	// Getter/Setter
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	void ComputeWorldTransform();
	const Matrix4& GetWorldTransform() const { return mWorldTransform; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), Math::Sin(mRotation)); }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	// ������Ʈ �߰�/����
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	// ������ ����
	State mState;
	// ��ȯ
	Matrix4 mWorldTransform;
	Vector2 mPosition;	// ������ �߽���
	float mScale;		// ������ ���� (100% �� ��� 1.0f)
	float mRotation;	// ȸ�� ���� (����)
	bool mRecomputeWorldTransform;

	// �� ���Ͱ� ������ ������Ʈ��
	std::vector<class Component*> mComponents;
	class Game* mGame;
};