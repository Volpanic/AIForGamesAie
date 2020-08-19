#include "PinkOrca.h"
#include "PlayerFish.h"
#include "LevelState.h"
#include "ObjectTracker.h"
#include "Path.h"
#include "FollowPathBehavior.h"
#include "WanderBehaviour.h"
#include "Numbers.h"
#include "raymath.h"

PinkOrca::PinkOrca(LevelState* level) : Agent::Agent(level)
{
	SetOrigin(27 / 2, 13 / 2);
	m_collider = new Collider();
	m_collider->Setup(this, 14, 8);


	AddComponent<Drawable>(new Drawable(m_level->GetResources()->GetTexture("spr_pink_orca.png"), 27, 13));

	SetFriction(90.0f);
	//SetBehaviour(new WanderBehaviour(250));

}

PinkOrca::~PinkOrca()
{

}

void PinkOrca::RefreshNearestClam(int pos)
{
	if (pos < 0 || pos >=  m_targetClams.size() || m_targetClams[pos]->HasPearl()) return;

	auto m_targetClam = m_level->GetObjectTracker()->GetNearest<Clam>(this, m_position, [&](Clam* object)
	{
		return (object->HasPearl() && std::find(m_targetClams.begin(), m_targetClams.end(), object) == m_targetClams.end());
	});

	if (!m_targetClams[pos]->HasPearl() && m_targetClam != nullptr)
	{
		m_targetClams[pos] = m_targetClam;
	}
	else
	{
		m_targetClams.erase(m_targetClams.begin()+pos);
		m_targetClamIndex = Clamp(m_targetClamIndex,0,m_targetClams.size());
	}
}


void PinkOrca::Update(float deltaTime)
{
	Agent::Update(deltaTime);

	if (m_firstRun)
	{
		//Get nearest clam with a pearl
		for (int i = 0; i < 3; i++)
		{
			auto m_targetClam = m_level->GetObjectTracker()->GetNearest<Clam>(this, m_position, [&](Clam* object)
			{
				return object->HasPearl() && std::find(m_targetClams.begin(), m_targetClams.end(), object) == m_targetClams.end();
			});
			m_targetClams.push_back(m_targetClam);
		}

		m_firstRun = false;
	}


	switch (m_currentState)
	{
	case SharkState::Movement:
	{
		if (m_behaviour == nullptr)
		{
			auto nearest = m_level->GetGraph()->GetNearestNode(m_position);
			auto clamNode = m_level->GetGraph()->GetNearestNode(m_targetClams[m_targetClamIndex]->GetPosition());

			if (nearest != nullptr && clamNode != nullptr)
			{
				auto nodeList = m_level->GetGraph()->ForEachAStar(nearest, clamNode, NULL);

				std::vector<Vector2> path;

				for (auto const& node : nodeList)
				{
					path.push_back(node->data);
				}

				SetBehaviour(new FollowPathBehavior(new Path(path), 50));
			}
			
			m_targetClamIndex += m_targetClamDirection;

			if (m_targetClamIndex > m_targetClams.size() || m_targetClamIndex < 0)
			{
				m_targetClamDirection = -m_targetClamDirection;
				m_targetClamIndex += m_targetClamDirection;
			}
			RefreshNearestClam(m_targetClamIndex);

			std::cout << "SOCOR: " + std::to_string(m_targetClamIndex) << std::endl;
		}

		//Check if can see player
		m_checkCanSeePlayerTimer += deltaTime;

		//Check every half second, opti.
		if (m_checkCanSeePlayerTimer >= 0.5f)
		{
			auto player = m_level->GetObjectTracker()->First<PlayerFish>();
			if (player != nullptr)
			{
				auto dist = Vector2Distance(m_position, player->GetPosition());

				if (dist < 128.0f)
				{
					Vector2 newDirec = Vector2Subtract(player->GetPosition(), m_position);
					if (m_collider->RaycastCheckAgainstSolids(atan2(newDirec.y, newDirec.x) * RAD2DEG, player->GetCollider().GetBBox(), m_level))
					{
						m_currentState = SharkState::Attack;
						Vector2 norm = Vector2Normalize(newDirec);

						m_velocity.x = norm.x * (dist * 2.0f);
						m_velocity.y = norm.y * (dist * 2.0f);
						std::cout << "Found" << std::endl;
					}
				}
			}
			m_checkCanSeePlayerTimer = 0.0f;
		}

		break;
	}

	case SharkState::Attack:
	{
		//m_velocity.x = Lerp(m_velocity.x, 0.0f, 1.0f * deltaTime);
		//m_velocity.y = Lerp(m_velocity.y, 0.0f, 1.0f * deltaTime);

		m_checkCanSeePlayerTimer += deltaTime;
		if (m_checkCanSeePlayerTimer >= 0.5f)
		{
			m_currentState = SharkState::Movement;
			m_checkCanSeePlayerTimer = 0.0f;
		}

		//Hurt Player
		auto player = m_level->GetObjectTracker()->First<PlayerFish>();
		if (player != NULL && m_collider->CollideAt(player->GetCollider(), m_position))
		{
			player->Hurt();
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

void PinkOrca::Draw()
{
	Agent::Draw();
}

void PinkOrca::DrawInEditor(const Vector2& mousePos)
{
	m_position = mousePos;
	Agent::Draw();
	DrawRectangleLinesEx(m_collider->GetBBox(), 1, GREEN);
}