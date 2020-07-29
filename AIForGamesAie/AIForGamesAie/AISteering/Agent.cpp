#include "Agent.h"
#include "SteeringBehaviour.h"

Agent::Agent(LevelState* level) : Actor::Actor(level)
{
	SetOrigin(8, 8);
	m_collider = new Collider();
	m_collider->Setup(this,16,16);

	AddComponent<Drawable>(new Drawable(m_level->GetResources()->GetTexture("spr_player.png"),16,16));
}

Agent::~Agent()
{
	delete m_behaviour;
}

void Agent::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

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

	m_rotation = atan2(m_velocity.y, m_velocity.x);

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

	DrawLineV(m_position,Vector2Add(m_position,Vector2Scale(m_velocity,0.25f)),DARKGRAY);

	GameObject::Draw();
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
