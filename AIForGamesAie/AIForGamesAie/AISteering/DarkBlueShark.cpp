#include "DarkBlueShark.h"
#include "PlayerFish.h"
#include "LevelState.h"
#include "ObjectTracker.h"
#include "Path.h"
#include "FollowPathBehavior.h"

DarkBlueShark::DarkBlueShark(LevelState* level) : Agent::Agent(level)
{
	SetOrigin(11, 6);
	m_collider = new Collider();
	m_collider->Setup(this, 24, 10);

	AddComponent<Drawable>(new Drawable(m_level->GetResources()->GetTexture("spr_dark_blue_shark.png"), 27, 13));
}

DarkBlueShark::~DarkBlueShark()
{

}

void DarkBlueShark::Update(float deltaTime)
{
	Agent::Update(deltaTime);

	if (m_behaviour == nullptr)
	{
		auto graph = m_level->GetGraph();

		if (graph != nullptr)
		{

			auto myNode = graph->GetNearbyNodes(m_position, 8)[0];
			Graph2D::Node* otherNode = nullptr;

			auto player = m_level->GetObjectTracker()->First<PlayerFish>();

			if (player != nullptr)
			{
				otherNode = graph->GetNearbyNodes(player->GetPosition(), 8)[0];
				auto nodePath = graph->ForEachAStar(myNode, otherNode, NULL);

				Path* newPath = new Path();

				for (auto const& node : nodePath)
				{
					newPath->PathAddNode(node->data);
				}

				SetBehaviour(new FollowPathBehavior(newPath, 100));
			}
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
