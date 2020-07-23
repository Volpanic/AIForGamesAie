#include "Actor.h"

Actor::Actor()
{
	AddComponent(new Drawable(LoadTexture("Sprites/spr_save.png"), 16, 16));
	//drawable = GetComponent<Drawable>();
}

Actor::~Actor()
{

}

void Actor::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
	MoveX(m_velocity.x);
	MoveY(m_velocity.y);
}

void Actor::Draw()
{
	GameObject::Draw();
}


void Actor::MoveX(float x)
{
	m_position.x += x;
}

void Actor::MoveY(float y)
{
	m_position.y += y;
}
