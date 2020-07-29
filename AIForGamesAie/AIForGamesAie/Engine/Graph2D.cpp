#include "Graph2D.h"
#include "Graph.h"
#include "raymath.h"

Graph2D::Graph2D()
{

}

Graph2D::~Graph2D()
{

}

std::vector<Graph2D::Node*> Graph2D::GetNearbyNodes(Vector2 position, float radius)
{
	std::vector<Graph2D::Node*> returnList;

	for (auto node : m_nodes)
	{
		float dist = Vector2Distance(position, node->data);

		if (dist <= radius)
		{
			returnList.push_back(node);
		}
	}
	return returnList;
}

