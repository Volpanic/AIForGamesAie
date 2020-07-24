#pragma once
#include "raylib.h"
#include "Behaviour.h"

class SteeringBehaviour : public Behaviour
{
public:
	SteeringBehaviour(const Vector2& targetPos,const float& speed);
	virtual ~SteeringBehaviour();

	virtual void Update(Agent* obj, float deltaTime);
	virtual void Draw(Agent* obj);

protected:
	Vector2 m_targetPosition;
	float m_speed;
};

