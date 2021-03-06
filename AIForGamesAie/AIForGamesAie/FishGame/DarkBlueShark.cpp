#include "DarkBlueShark.h"
#include "PlayerFish.h"
#include "LevelState.h"
#include "ObjectTracker.h"
#include "Path.h"
#include "FollowPathBehavior.h"
#include "WanderBehaviour.h"
#include "Numbers.h"
#include "raymath.h"

DarkBlueShark::DarkBlueShark(LevelState* level) : Agent::Agent(level)
{
	SetOrigin(27/2, 13/2);
	m_collider = new Collider();
	m_collider->Setup(this, 14, 8);


	AddComponent<Drawable>(new Drawable(m_level->GetResources()->GetTexture("spr_dark_blue_shark.png"), 27, 13));

	SetFriction(90.0f);
	//SetBehaviour(new WanderBehaviour(250));
}

DarkBlueShark::~DarkBlueShark()
{

}

void DarkBlueShark::Update(float deltaTime)
{
	Agent::Update(deltaTime);

	switch (m_currentState)
	{
		case SharkState::Movement:
		{
			if (m_behaviour == nullptr)
			{
				auto nearest = m_level->GetGraph()->GetNearestNode(m_position);
				auto randNode = m_level->GetGraph()->GetRandomNode();

				if (nearest != nullptr && randNode != nullptr)
				{
					auto nodeList = m_level->GetGraph()->ForEachAStar(nearest, randNode, NULL);

					std::vector<Vector2> path;

					for (auto const& node : nodeList)
					{
						path.push_back(node->data);
					}

					SetBehaviour(new FollowPathBehavior(new Path(path), 50));
				}
			}

			//Check if can see player
			m_checkCanSeePlayerTimer += deltaTime;

			//Check every half second, opti.
			if (m_checkCanSeePlayerTimer >= 0.5f)
			{
				auto player = m_level->GetObjectTracker()->First<PlayerFish>();
				if (player != nullptr)
				{
					Vector2 newDirec = Vector2Subtract(player->GetPosition(), m_position);
					if (m_collider->RaycastCheckAgainstSolids(atan2(newDirec.y, newDirec.x) * RAD2DEG, player->GetCollider().GetBBox(), m_level))
					{
						m_currentState = SharkState::Attack;
						Vector2 norm = Vector2Normalize(newDirec);
						m_velocity.x = norm.x * 200;
						m_velocity.y = norm.y * 200;
					}
				}
				m_checkCanSeePlayerTimer = 0.0f;
			}

			break;
		}

		case SharkState::Attack:
		{
			m_checkCanSeePlayerTimer += deltaTime;
			if (m_checkCanSeePlayerTimer >= 1.5f)
			{
				m_currentState = SharkState::Movement;
				m_checkCanSeePlayerTimer = 0.0f;
			}
			
			//Hurt Player
			auto player = m_level->GetObjectTracker()->First<PlayerFish>();
			if (player != NULL)
			{
				Vector2 newDirec = Vector2Subtract(player->GetPosition(), m_position);
				Vector2 norm = Vector2Normalize(newDirec);
				m_velocity.x = norm.x*75;
				m_velocity.y = norm.y*75;

				if (m_collider->CollideAt(player->GetCollider(), m_position))
				{
					player->Hurt();
				}
			}

			break;
		}
	}

	//Flip to face velocitty
	if (m_velocity.x != 0)
	{
		m_scale.x = Numbers::Sign<float>(m_velocity.x);
	}
}

void DarkBlueShark::Draw()
{
	Agent::Draw();
}

void DarkBlueShark::DrawInEditor(const Vector2& mousePos)
{
	m_position = mousePos;
	Agent::Draw();
	DrawRectangleLinesEx(m_collider->GetBBox(), 1, GREEN);
}
