#include "PlayerFish.h"
#include "Numbers.h"
#include "ObjectTracker.h"

PlayerFish::PlayerFish(LevelState* level) : Agent::Agent(level)
{
	SetOrigin(17/2, 9/2);
	m_collider = new Collider();
	m_collider->Setup(this, 15, 7);

	AddComponent<Drawable>(new Drawable(m_level->GetResources()->GetTexture("spr_player_fish.png"), 17, 9));

	m_drawable = GetComponent<Drawable>();
	SetFriction(0.0f);

}

PlayerFish::~PlayerFish()
{

}

void PlayerFish::Update(float deltaTime)
{
	auto KeyUp = IsKeyDown(KEY_UP);
	auto KeyDown = IsKeyDown(KEY_DOWN);
	auto KeyLeft = IsKeyDown(KEY_LEFT);
	auto KeyRight = IsKeyDown(KEY_RIGHT);

	float drag = 250.0f * deltaTime;

	if (KeyUp)
	{
		m_velocity.y = Numbers::Approach(m_velocity.y, -100.0f, drag);
	}

	if (KeyDown)
	{
		m_velocity.y = Numbers::Approach(m_velocity.y, 150.0f, drag);
	}

	if (!KeyDown && !KeyUp)
	{
		m_velocity.y = Numbers::Approach(m_velocity.y,10.f, drag);
	}

	if (KeyLeft)
	{
		m_velocity.x = Numbers::Approach(m_velocity.x, -150.0f, drag);
	}

	if (KeyRight)
	{
		m_velocity.x = Numbers::Approach(m_velocity.x, 150.0f, drag);
	}

	if (!KeyRight && !KeyLeft)
	{
		m_velocity.x = Numbers::Approach(m_velocity.x, 0.0f, drag);
	}
	
	//m_velocity.x = Clamp(m_velocity.x, -150, 150);
	//m_velocity.y = Clamp(m_velocity.y, -150, 150);

	if (m_velocity.x != 0)
	{
		m_scale.x = Numbers::Sign<float>(m_velocity.x);
	}

	MoveX(m_velocity.x * deltaTime);
	MoveY(m_velocity.y * deltaTime);

	m_level->SetCameraPositoin(Vector2Subtract(m_position, {320/2.0f,180/2.0f}));
	//m_level->SetCameraPositoin(Vector2Subtract(m_position, {320.0f,180.0f}));

	m_hitTimer = Numbers::Approach(m_hitTimer,0.0f,deltaTime);
	if (m_hitTimer == 0)
	{
		m_recentlyHit = false;
	}

	//Get nearest clam with a pearl
	m_nearestClam = m_level->GetObjectTracker()->GetNearest<Clam>(this, m_position,[](Clam* object) 
	{
		return object->HasPearl();
	});
}

void PlayerFish::Draw()
{
	if (!m_recentlyHit)
	{
		Agent::Draw();
	}
	else
	{
		if (fmod(m_hitTimer,0.10f) <= 0.05f)
		{
			Agent::Draw();
		}
	}

	if (m_nearestClam != nullptr)
	{
		Vector2 direction = Vector2Normalize(Vector2Subtract(m_nearestClam->GetPosition(), m_position));
		float dir = atan2(direction.y,direction.x);
		m_nearestClamRotation = Lerp(m_nearestClamRotation,dir,0.08f);
		//Vector2 arrowPos = {16*direction.x,16*direction.y};
		Vector2 arrowPos = {0,-16.0f};

		DrawTexturePro(m_level->GetResources()->GetTexture("spr_arrow.png"), { 0,0,14,13 }, { m_position.x + arrowPos.x,m_position.y + arrowPos.y,14,13}, { 14.0f / 2,13.0f / 2 },Numbers::FloorMultiple(m_nearestClamRotation *RAD2DEG,45), WHITE);
	}
}

void PlayerFish::DrawInEditor(const Vector2& mousePos)
{
	m_position = mousePos;
	Agent::Draw();
	DrawRectangleLinesEx(m_collider->GetBBox(), 1 , GREEN);
}

void PlayerFish::Hurt()
{
	if (!m_recentlyHit)
	{
		m_recentlyHit = true;
		m_hitTimer = 1.5f;
	}
}
