#include "PlayerFish.h"
#include "Numbers.h"

PlayerFish::PlayerFish(LevelState* level) : Agent::Agent(level)
{
	SetOrigin(17/2, 9/2);
	m_collider = new Collider();
	m_collider->Setup(this, 17, 9);

	AddComponent<Drawable>(new Drawable(m_level->GetResources()->GetTexture("spr_player_fish.png"), 17, 9));

	m_drawable = GetComponent<Drawable>();
}

PlayerFish::~PlayerFish()
{

}

void PlayerFish::Update(float deltaTime)
{
	SetFriction(2.0f);

	if (IsKeyDown(KEY_UP))
	{
		ApplyForce({0,-50});
	}

	if (IsKeyDown(KEY_DOWN))
	{
		ApplyForce({ 0,50 });
	}

	if (IsKeyDown(KEY_LEFT))
	{
		ApplyForce({-50,0 });
	}

	if (IsKeyDown(KEY_RIGHT))
	{
		ApplyForce({ 50,0 });
	}

	Agent::Update(deltaTime);

	m_velocity.x = Clamp(m_velocity.x, -250, 250);
	m_velocity.y = Clamp(m_velocity.y, -250, 250);

	if (m_velocity.x != 0)
	{
		m_scale.x = Numbers::Sign<float>(m_velocity.x);
	}
}

void PlayerFish::Draw()
{
	Agent::Draw();
}

void PlayerFish::DrawInEditor(const Vector2& mousePos)
{
	m_position = mousePos;
	Agent::Draw();
	DrawRectangleLinesEx(m_collider->GetBBox(), 1, GREEN);
}
