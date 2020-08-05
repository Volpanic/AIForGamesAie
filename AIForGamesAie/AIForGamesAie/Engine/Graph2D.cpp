#include "Graph2D.h"
#include "Graph.h"
#include "raymath.h"
#include <iostream>
#include <functional>
#include <map>

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

std::vector<Graph2D::Node*> Graph2D::ForEachDijkstra(Graph2D::Node* startNode, Graph2D::Node* endNode, std::function<void(Graph2D::Node * n)> process)
{
	std::list<Graph2D::PathfindNode* > openList;
	std::map<Graph2D::Node*,Graph2D::PathfindNode* > closedList;

	std::vector<Node*> returnNodes;

	openList.push_front(new Graph2D::PathfindNode({ startNode ,nullptr,0 }));

	while (!openList.empty())
	{
		Graph2D::PathfindNode* current = openList.front();
		openList.pop_front();
		closedList[current->graphNode] = current;

		//End
		if (current->graphNode == endNode)
		{
			std::cout << current->gScore << std::endl;
			while (current->parent != nullptr)
			{
				returnNodes.push_back(current->graphNode);
				current = current->parent;
			}
			returnNodes.push_back(current->graphNode);

			std::reverse(returnNodes.begin(), returnNodes.end());
			return returnNodes;
		}

		//Edges
		for (auto const& edge : current->graphNode->connections)
		{
			bool doAdd = true;

			//Check Q
			for (auto const& opened : openList)
			{
				if (opened ->graphNode == edge.to)
				{
					doAdd = false;
					break;
				}
			}

			//Check closed
			if(closedList.find(edge.to) != closedList.end())
			{
				PathfindNode* closed = closedList[edge.to];
				if (closed->gScore > current->gScore + edge.data)
				{
					closed->gScore = current->gScore + edge.data;
					closed->parent = current;
				}
				doAdd = false;
			}
			

			if (doAdd)
			{
				openList.push_front(new Graph2D::PathfindNode({ edge.to ,current,current->gScore + edge.data }));
			}
		}


		openList.sort([](const Graph2D::PathfindNode* a, const Graph2D::PathfindNode* b) {
			return a->gScore < b->gScore;
		});
	
	}

	return returnNodes;
}

std::vector<Graph2D::Node*> Graph2D::ForEachAStar(Graph2D::Node* startNode, Graph2D::Node* endNode, std::function<void(Graph2D::Node * n)> process)
{
	std::list<Graph2D::PathfindNode* > openList;
	std::map<Graph2D::Node*, Graph2D::PathfindNode* > closedList;

	std::vector<Node*> returnNodes;

	openList.push_front(new Graph2D::PathfindNode({ startNode ,nullptr,0 }));

	while (!openList.empty())
	{
		Graph2D::PathfindNode* current = openList.front();
		openList.pop_front();
		closedList[current->graphNode] = current;

		//End
		if (current->graphNode == endNode)
		{
			std::cout << current->gScore << std::endl;
			while (current->parent != nullptr)
			{
				returnNodes.push_back(current->graphNode);
				current = current->parent;
			}
			returnNodes.push_back(current->graphNode);

			std::reverse(returnNodes.begin(), returnNodes.end());
			return returnNodes;
		}

		//Edges
		for (auto const& edge : current->graphNode->connections)
		{
			bool doAdd = true;

			//Check Q
			for (auto const& opened : openList)
			{
				if (opened->graphNode == edge.to)
				{
					doAdd = false;
					break;
				}
			}

			//Check closed
			if (closedList.find(edge.to) != closedList.end())
			{
				PathfindNode* closed = closedList[edge.to];
				if (closed->gScore > current->gScore + edge.data)
				{
					closed->gScore = (current->gScore + edge.data + Vector2Distance(edge.to->data, endNode->data));
					closed->parent = current;
				}
				doAdd = false;
			}


			if (doAdd)
			{
				openList.push_front(new Graph2D::PathfindNode({ edge.to ,current,(current->gScore + edge.data + Vector2Distance(edge.to->data,endNode->data))}));
			}
		}


		openList.sort([](const Graph2D::PathfindNode* a, const Graph2D::PathfindNode* b) {
			return a->gScore < b->gScore;
		});

	}

	return returnNodes;
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

