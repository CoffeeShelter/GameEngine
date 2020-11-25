#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"
#include "CircleComponent.h"
#include "Asteroid.h"
#include "Math.h"

Ship::Ship(Game* game)
	:Actor(game)
	, mLaserCooldown(0.0f)
	, mCircle(nullptr)
{
	// ��������Ʈ ������Ʈ ����
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// �Է� ������Ʈ ����/����
	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);

	// CircleComponent ����
	// collision
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);
}

void Ship::UpdateActor(float deltaTime) {
	// ������ ��Ÿ��
	mLaserCooldown -= deltaTime;

	for (auto ast : GetGame()->GetAsteroids())
	{
		if (Intersect(*mCircle, *(ast->GetCircle())))
		{
			SetPosition(Vector2(512.0f, 384.0f));
			SetRotation(0.0f);
		}
	}


}

void Ship::ActorInput(const uint8_t* keyState) {
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f) {
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		mLaserCooldown = 0.5f;
	}
}