#include "SteeringBehaviour.h"
#include "Agent.h"

SteeringBehaviour::SteeringBehaviour(const Vector2& targetPos, const float& speed) : m_targetPosition(targetPos), m_speed(speed)
{
	
}

SteeringBehaviour::~SteeringBehaviour()
{

}

void SteeringBehaviour::Update(Agent* obj, float deltaTime)
{
	Vector2 heading = Vector2Add(obj->GetPosition(), obj->GetPosition());
	Vector2 dirvec = Vector2Subtract(m_targetPosition, obj->GetPosition());

	float dir = atan2(dirvec.y, dirvec.x);

	Vector2 force = { cos(dir),sin(dir) };

	obj->ApplyForce(Vector2Scale(force, m_speed));

	if (Vector2Distance(obj->GetPosition(), m_targetPosition) < (m_speed * deltaTime))
	{
		obj->PopBehaviour();
	}
}

void SteeringBehaviour::Draw(Agent* obj)
{
	DrawCircleV(m_targetPosition, 2, { 128,128,0,64 });
	DrawCircleV(m_targetPosition, 8, { 255,128,0,64 });
}
