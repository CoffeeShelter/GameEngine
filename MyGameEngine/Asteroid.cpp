#include "Asteroid.h"
#include "CircleComponent.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"

Asteroid::Asteroid(Game* game)
	:Actor(game)
	, mCircle(nullptr)
{
	// 랜덤값으로 위치 초기화
	Vector2 randPos = Random::GetVector(Vector2(-512.0f,-384.0f), Vector2(512.0f, 384.0f));
	SetPosition(randPos);

	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

	// SpriteComponent 추가
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	// MoveComponent 추가
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.0f);

	// CircleComponent 추가
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);

	// 게임에 해당 객체 추가
	game->AddAsteroid(this);
}

Asteroid::~Asteroid() {
	GetGame()->RemoveAsteroid(this);
}