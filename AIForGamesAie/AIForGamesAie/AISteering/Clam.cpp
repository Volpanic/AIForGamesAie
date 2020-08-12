#include "Clam.h"
#include "LevelState.h"
#include "PlayerFish.h";
#include "ObjectTracker.h"

Clam::Clam(LevelState* level) : m_level(level)
{
	m_collider = new Collider();
	m_collider->Setup(this, 16, 16);


	AddComponent<Drawable>(new Drawable(m_level->GetResources()->GetTexture("spr_clam.png"), 16, 16));
	m_drawable = GetComponent<Drawable>();
}

Clam::~Clam()
{
}

void Clam::Update(float deltaTime)
{
	if (m_hasPearl)
	{
		auto player = m_level->GetObjectTracker()->First<PlayerFish>();

		if (player != nullptr)
		{
			if (m_collider->CollideAt(player->GetCollider(), m_position))
			{
				m_hasPearl = false;
				m_drawable->SetDrawRect(1);
			}
		}
	}

	GameObject::Update(deltaTime);
}

void Clam::Draw()
{
	GameObject::Draw();
}

void Clam::DrawInEditor(const Vector2& mousePos)
{
	m_position = mousePos;
	GameObject::Draw();
	DrawRectangleLinesEx(m_collider->GetBBox(), 1, GREEN);
}
