#include "WanderBehaviour.h"
#include "Agent.h"
#include "Path.h"

WanderBehaviour::WanderBehaviour(const float& speed) : m_speed(speed)
{
	m_direction = GetRandomValue(0, 360);
}

WanderBehaviour::~WanderBehaviour()
{
	
}

void WanderBehaviour::Update(Agent* obj, float deltaTime)
{
	m_direction = GetRandomValue(0,360);
	float dist = 32;

	float xOff = cos(m_direction) * dist;
	float yOff = sin(m_direction) * dist;

	MoveTowards(obj, {obj->GetPosition().x + xOff,obj->GetPosition().y + yOff},deltaTime);
}

bool WanderBehaviour::MoveTowards(Agent* obj, Vector2 target, float deltaTime)
{
	Vector2 heading = Vector2Add(obj->GetPosition(), obj->GetPosition());
	Vector2 dirvec = Vector2Subtract(target, obj->GetPosition());

	float dir = atan2(dirvec.y, dirvec.x);

	Vector2 force = { cos(dir),sin(dir) };

	obj->ApplyForce(Vector2Scale(force, m_speed));

	if (Vector2Distance(obj->GetPosition(), target) < (obj->GetCollider().BBoxWidth()))
	{
		return true;
	}
	return false;
}

void WanderBehaviour::Draw(Agent* obj)
{
	
}
