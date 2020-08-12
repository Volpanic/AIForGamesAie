#include "DarkBlueShark.h"
#include "PlayerFish.h"
#include "LevelState.h"
#include "ObjectTracker.h"
#include "Path.h"
#include "FollowPathBehavior.h"
#include "WanderBehaviour.h"
#include "Numbers.h"

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
		case 0:
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

			if (m_velocity.x != 0)
			{
				m_scale.x = Numbers::Sign<float>(m_velocity.x);
			}
			break;
		}
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
