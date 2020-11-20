#include "Math.h"
#include <vector>

class Actor {
public:
	// 액터의 상태를 추적하는데 사용된다.
	enum State {
		EActive,
		EPaused,
		EDead
	};
	// 생성자/소멸자
	Actor(class Game* game);
	virtual ~Actor();

	// Game에서 호출하는 Update 함수 (가상 함수 아님)
	void Update(float deltaTime);
	// 액터에 부착된 모든 컴포넌트를 업데이트 (가상 함수 아님)
	void UpdateComponents(float deltaTime);
	// 특정 액터에 특화된 업데이트 코드 (오버라이딩 가능)
	virtual void UpdateActor(float deltaTime);

	// Getter/Setter

	// 컴포넌트 추가/제거
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	// 액터의 상태
	State mState;
	// 변환
	Vector2 mPosition;	// 액터의 중심점
	float mScale;		// 액터의 배율 (100% 의 경우 1.0f)
	float mRotation;	// 회전 각도 (라디안)
	// 이 액터가 보유한 컴포넌트들
	std::vector<class Component*> mComponents;
	class Game* mGame;
};