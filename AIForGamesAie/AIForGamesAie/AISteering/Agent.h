#pragma once
#include "Actor.h"

class Agent : public Actor
{
public:
	
	Agent(LevelState* level);
	virtual ~Agent();
	virtual void Update(float deltaTime);
	virtual void Draw();

	//setters
	void SetFriction(float fricc) { m_friciton = fricc;};
	void ApplyForce(const Vector2& force);
	void SetTargetPosition(const Vector2& targetPos) { m_targetPos = targetPos; };

protected:
	void MoveToward(const Vector2& targetPos, float speed);

private:
	Vector2 m_acceleration = { 0,0 };
	Vector2 m_targetPos = { 0,0 };
	float m_friciton = 0;
};

