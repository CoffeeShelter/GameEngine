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
	// ���������� ��ġ �ʱ�ȭ
	Vector2 randPos = Random::GetVector(Vector2(-512.0f,-384.0f), Vector2(512.0f, 384.0f));
	SetPosition(randPos);

	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

	// SpriteComponent �߰�
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	// MoveComponent �߰�
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.0f);

	// CircleComponent �߰�
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);

	// ���ӿ� �ش� ��ü �߰�
	game->AddAsteroid(this);
}

Asteroid::~Asteroid() {
	GetGame()->RemoveAsteroid(this);
}