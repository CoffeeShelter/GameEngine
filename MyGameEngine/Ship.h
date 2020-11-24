#pragma once
#include "Actor.h"

class Ship : public Actor {
public:
	Ship(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;
private:
	// 레이저 쿨 타임
	float mLaserCooldown;
};