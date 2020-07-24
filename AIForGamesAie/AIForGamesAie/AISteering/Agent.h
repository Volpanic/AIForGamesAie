#pragma once
#include "Actor.h"
#include "Behaviour.h"

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
	void SetBehaviour(Behaviour* behaviour);
	void PopBehaviour();

protected:
	Behaviour* m_behaviour = nullptr;

private:
	Vector2 m_acceleration = { 0,0 };
	float m_friciton = 0;
};

