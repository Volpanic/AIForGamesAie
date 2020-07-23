#pragma once
#include "Actor.h"

class Agent : public Actor
{
public:
	virtual ~Agent();
	virtual void Update(float deltaTime);
	virtual void Draw();

	//setters
	void SetFriction(float fricc) { m_friciton = fricc;};
	
	void ApplyForce(const Vector2& force);
	void MoveToward(const Vector2& targetPos, float speed);

private:
	Vector2 m_acceleration = { 0,0 };
	float m_friciton = {};
};

