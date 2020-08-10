#pragma once
#include "Behaviour.h"
#include "raylib.h"

class Path;

class WanderBehaviour : public Behaviour
{
public:
	WanderBehaviour(const float& speed);
	virtual ~WanderBehaviour();

	bool MoveTowards(Agent* obj, Vector2 target, float deltaTime);
	virtual void Update(Agent* obj, float deltaTime);
	virtual void Draw(Agent* obj);

protected:
	float m_speed;
	float m_direction = 0;
};

