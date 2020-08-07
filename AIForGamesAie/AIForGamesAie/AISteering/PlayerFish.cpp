#include "PlayerFish.h"
#include "Numbers.h"

PlayerFish::PlayerFish(LevelState* level) : Agent::Agent(level)
{
	SetOrigin(17/2, 9/2);
	m_collider = new Collider();
	m_collider->Setup(this, 17, 9);

	AddComponent<Drawable>(new Drawable(m_level->GetResources()->GetTexture("spr_player_fish.png"), 17, 9));

	m_drawable = GetComponent<Drawable>();
	SetFriction(2.0f);

}

PlayerFish::~PlayerFish()
{

}

void PlayerFish::Update(float deltaTime)
{
	
	Agent::Update(deltaTime);

	if (IsKeyDown(KEY_UP))
	{
		ApplyForce({0,-200});
	}

	if (IsKeyDown(KEY_DOWN))
	{
		ApplyForce({ 0,200 });
	}

	if (IsKeyDown(KEY_LEFT))
	{
		ApplyForce({-200,0 });
	}

	if (IsKeyDown(KEY_RIGHT))
	{
		ApplyForce({ 200,0 });
	}

	
	m_velocity.x = Clamp(m_velocity.x, -150, 150);
	m_velocity.y = Clamp(m_velocity.y, -150, 150);

	if (m_velocity.x != 0)
	{
		m_scale.x = Numbers::Sign<float>(m_velocity.x);
	}

	m_level->SetCameraPositoin(Vector2Subtract(m_position, {320/2.0f,180/2.0f}));
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
