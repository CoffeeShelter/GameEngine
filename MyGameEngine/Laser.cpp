#include "Game.h"
#include "Laser.h"
#include "Asteroid.h"
#include "CircleComponent.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"

Laser::Laser(Game* game) 
	:Actor(game)
	,mCircle(nullptr)
	,mDeathTimer(1.0f)
{
	// SpriteComponent 추가
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Laser.png"));

	// MoveComponent 추가
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(800.0f);

	// CircleComponent 추가
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(11.0f);
}

void Laser::UpdateActor(float deltaTime) {
	mDeathTimer -= deltaTime;
	if (mDeathTimer <= 0.0f) {
		SetState(EDead);
	}
	else 
	{
		for (auto ast : GetGame()->GetAsteroids()) 
		{
			if(Intersect(*mCircle,*(ast->GetCircle())))
			{
				SetState(EDead);
				ast->SetState(EDead);
				break;
			}
		}
	}
}