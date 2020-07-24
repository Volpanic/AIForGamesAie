#include "Agent.h"


Agent::Agent(LevelState* level)
{
	m_level = level; 
	SetOrigin(8, 8);
	m_collider = new Collider();
	m_collider->Setup(this,16,16);
}

Agent::~Agent()
{

}

void Agent::Update(float deltaTime)
{
	

	SetFriction(2.0f);

	if (IsKeyDown(KEY_A)) ApplyForce({ -100, 0 });
	if (IsKeyDown(KEY_D)) ApplyForce({  100, 0 });
	if (IsKeyDown(KEY_W)) ApplyForce({  0,  -100 });
	if (IsKeyDown(KEY_S)) ApplyForce({  0,   100 });

	//MoveToward(m_targetPos,250);

	ApplyForce(Vector2Scale(Vector2Negate(m_velocity), m_friciton));

	m_velocity = Vector2Add(m_velocity, Vector2Scale(m_acceleration, deltaTime));

	MoveX(m_velocity.x * deltaTime);
	MoveY(m_velocity.y * deltaTime);

	m_acceleration = Vector2Zero();
}

void Agent::Draw()
{
	DrawCircleV(m_position,8,DARKGRAY);
	DrawLineV(m_position,Vector2Add(m_position,m_velocity),DARKGRAY);

	DrawCircleV(GetCollider().BBoxTopLeft(), 2, RED);
	DrawCircleV(GetCollider().BBoxTopRight(), 2, RED);
	DrawCircleV(GetCollider().BBoxBottomLeft(), 2, RED);
	DrawCircleV(GetCollider().BBoxBottomRight(), 2, RED);

	DrawRectangleRec(GetCollider().GetBBox(), { 128,128,128,128 });
}

void Agent::ApplyForce(const Vector2& force)
{
	m_acceleration = Vector2Add(m_acceleration,force);
}

void Agent::MoveToward(const Vector2& targetPos, float speed)
{
	Vector2 heading = Vector2Add(m_position,m_velocity);
	Vector2 dirvec = Vector2Subtract(targetPos, m_position);

	float dir = atan2(dirvec.y,dirvec.x);

	Vector2 force = { cos(dir),sin(dir)};

	ApplyForce(Vector2Scale(force,speed));
}