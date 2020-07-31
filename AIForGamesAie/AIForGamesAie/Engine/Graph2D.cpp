#include "Graph2D.h"
#include "Graph.h"
#include "raymath.h"
#include <functional>

Graph2D::Graph2D()
{

}

Graph2D::Graph2D(std::vector <Graph2D::Node*> nodes)
{
	m_nodes.clear();
	for (auto const& node : nodes)
	{
		m_nodes.push_back(node);
	}
}

Graph2D::~Graph2D()
{

}

std::vector<Graph2D::Node*>  Graph2D::ForEachDijkstra(Graph2D::Node* startNode, Graph2D::Node* endNode, std::function<void(Graph2D::Node * n)> process)
{
	std::vector<Graph2D::PathfindNode*> openList;
	std::vector<Graph2D::Node* > closedList;

	openList.push_back(new PathfindNode({ startNode ,nullptr,0 }));

	int gScore = 0;
	while (!openList.empty())
	{
		Graph2D::PathfindNode* node = openList.back();
		openList.pop_back();
		closedList.push_back(node->graphNode);

		//process(node);

		for (auto const& edge : node->graphNode->connections)
		{
			//Check if All ready
			bool doAdd = true;
			for (auto const& onOpenList : openList)
			{
				if (onOpenList->graphNode == edge.to)
				{
					doAdd = false;
				}
			}

			for (auto const& onClosedList : closedList)
			{
				if (onClosedList == edge.to)
				{
					doAdd = false;
				}
			}

			if(doAdd)
			{
				openList.push_back(new PathfindNode({ edge.to,node,edge.data }));

				if (edge.to == endNode)
				{
					std::vector<Graph2D::Node*> nodesToReturn;
					Graph2D::PathfindNode* check = openList.back();

					while (check->parent != nullptr)
					{
						nodesToReturn.push_back(check->graphNode);
						check = check->parent;
					}
					nodesToReturn.push_back(check->graphNode);
					return nodesToReturn;
				}
			}
		}
		std::sort(openList.begin(), openList.end());

	}
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

