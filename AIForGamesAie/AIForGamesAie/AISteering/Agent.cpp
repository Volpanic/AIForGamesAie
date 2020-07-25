#include "Agent.h"
#include "SteeringBehaviour.h"

Agent::Agent(LevelState* level)
{
	m_level = level; 
	SetOrigin(8, 8);
	m_collider = new Collider();
	m_collider->Setup(this,16,16);
}

Agent::~Agent()
{
	delete m_behaviour;
}

void Agent::Update(float deltaTime)
{
	if (m_behaviour != nullptr)
	{
		m_behaviour->Update(this, deltaTime);
	}

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		SetBehaviour(new SteeringBehaviour(m_level->GetScaledMousePos(),250));
	}


	SetFriction(3.0f);
	ApplyForce(Vector2Scale(Vector2Negate(m_velocity), m_friciton));

	//Collisions and stuff
	m_velocity = Vector2Add(m_velocity, Vector2Scale(m_acceleration, deltaTime));
	MoveX(m_velocity.x * deltaTime);
	MoveY(m_velocity.y * deltaTime);

	m_acceleration = Vector2Zero();
}

void Agent::Draw()
{
	if (m_behaviour != nullptr)
	{
		m_behaviour->Draw(this);
	}

	DrawCircleV(m_position,8,DARKGRAY);
	DrawLineV(m_position,Vector2Add(m_position,m_velocity),DARKGRAY);
}

void Agent::SetBehaviour(Behaviour* behaviour)
{
	if (m_behaviour != nullptr)
	{
		delete m_behaviour;
	}
	m_behaviour = behaviour;
}

void Agent::PopBehaviour()
{
	delete m_behaviour;
	m_behaviour = nullptr;
}

void Agent::ApplyForce(const Vector2& force)
{
	m_acceleration = Vector2Add(m_acceleration,force);
}
