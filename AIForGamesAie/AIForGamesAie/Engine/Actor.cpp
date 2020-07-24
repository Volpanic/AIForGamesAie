#include "Actor.h"
#include "raymath.h"
#include "Numbers.h"

Actor::Actor()
{

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
	m_subPixelMovement.x += x;
	int move = round(m_subPixelMovement.x);

	std::list<Rectangle> solids = m_level->GetSolids(GetCollider().GetBBox(), Vector2Add(m_position,{x,0}));

	if (move != 0)
	{
		m_subPixelMovement.x -= move;
		int sign = Numbers::Sign(move);

		while (move != 0)
		{
			if (solids.empty() || !GetCollider().CollideAt(solids, Vector2Add(m_position, { (float)sign,0 })))
			{
				m_position.x += sign;
				move -= sign;
			}
			else
			{
				m_velocity.x = 0;
				m_subPixelMovement.x = 0;
				break;
			}
		}
	}
}

void Actor::MoveY(float y)
{
	m_subPixelMovement.y += y;
	int move = round(m_subPixelMovement.y);

	std::list<Rectangle> solids = m_level->GetSolids(GetCollider().GetBBox(), Vector2Add(m_position, { 0,y }));

	if (move != 0)
	{
		m_subPixelMovement.y -= move;
		int sign = Numbers::Sign(move);

		while (move != 0)
		{
			if (solids.empty() || !GetCollider().CollideAt(solids, Vector2Add(m_position, {0,(float)sign })))
			{
				m_position.y += sign;
				move -= sign;
			}
			else
			{
				m_velocity.y = 0;
				m_subPixelMovement.y = 0;
				break;
			}
		}
	}
}
