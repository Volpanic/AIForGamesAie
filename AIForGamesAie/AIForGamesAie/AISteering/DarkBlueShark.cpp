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
	SetOrigin(11, 6);
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

				auto nodeList = m_level->GetGraph()->ForEachAStar(nearest, randNode, NULL);

				std::vector<Vector2> path;

				for (auto const& node : nodeList)
				{
					path.push_back(node->data);
				}

				SetBehaviour(new FollowPathBehavior(new Path(path), 50));
			}

			//Check if can see player
			m_checkCanSeePlayerTimer += deltaTime;

			//Check every half second, opti.
			if (m_checkCanSeePlayerTimer >= 0.5f)
			{
				auto player = m_level->GetObjectTracker()->First<PlayerFish>();
				Vector2 newDirec = Vector2Subtract(player->GetPosition(), m_position);
				if (m_collider->RaycastCheckAgainstSolids(atan2(newDirec.y, newDirec.x) * RAD2DEG, player->GetCollider().GetBBox(), m_level))
				{
					m_currentState = SharkState::Attack;
					Vector2 norm = Vector2Normalize(newDirec);
					m_velocity.x = norm.x * 200;
					m_velocity.y = norm.y * 200;
					std::cout << "Found" << std::endl;
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
			break;
		}
	}

	//Flip to face velocitty
	if (m_velocity.x != 0)
	{
		m_scale.x = Numbers::Sign<float>(m_velocity.x);
	}

	//Rotate
	m_rotation = atan2(m_velocity.y, m_velocity.x);
	if (m_scale.x < 0) { m_rotation = 180 - m_rotation; }
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
