#pragma once
#include <vector>
#include <functional>
#include <set>
#include <queue>

template<class TNodeData, class TEdgeData>
class Graph
{
public:

	struct Node;
	struct Edge;

	struct Node
	{
		TNodeData data;
		std::vector<Edge> connections;
	};

	struct Edge
	{
		Node* to;
		TEdgeData data;
	};

public:

	// == == == == == == == == == == == == 
	// Constructor
	// == == == == == == == == == == == == 
	Graph() 
	{

	}

	// == == == == == == == == == == == == 
	// deconstructor
	// == == == == == == == == == == == == 
	virtual ~Graph()
	{
		for (auto const& node : m_nodes)
		{
			delete node;
		}
	}

	// == == == == == == == == == == == == 
	// Add Node
	// == == == == == == == == == == == == 
	Node* AddNode(const TNodeData& data)
	{
		Node* node = new Node();
		node->data = data;
	
		m_nodes.push_back(node);
		return node;
	}

	// == == == == == == == == == == == == 
	// Add Edge
	// == == == == == == == == == == == == 
	void AddEdge(Node* nodeFrom, Node* nodeTo, const TEdgeData& data)
	{
		nodeFrom->connections.push_back({ nodeTo,data });
	}

	// == == == == == == == == == == == == 
	// Get Nodes
	// == == == == == == == == == == == == 
	const std::vector<Node*>& GetNodes() { return m_nodes; };

	// == == == == == == == == == == == == 
	// Get Node
	// == == == == == == == == == == == == 
	Node* GetNode(int index) { return m_nodes[index]; };

	// == == == == == == == == == == == == 
	// Connect nodes
	// == == == == == == == == == == == == 
	void ConnectNodes(Node* nodeA, Node* nodeB,const TEdgeData& data)
	{
		AddEdge(nodeA, nodeB, data);
		AddEdge(nodeB, nodeA, data);
	}

	// == == == == == == == == == == == == 
	// Remove nodes
	// == == == == == == == == == == == == 
	void RemoveNode(Node* node)
	{
		for (auto nd : node->connections)
		{
			for (int i = 0; i < nd.to->connections.size(); i++)
			{
				if (nd.to->connections[i].to == node)
				{
					nd.to->connections.erase(nd.to->connections.begin()+i);
					break;
				}
			}
		}

		for (int i = 0; i < m_nodes.size(); i++)
		{
			if (m_nodes[i] == node)
			{
				m_nodes.erase(m_nodes.begin()+i);
				break;
			}
		}
		
		delete node;
	}

	// == == == == == == == == == == == == 
	// ForEachDFS
	// == == == == == == == == == == == == 
	void ForEachDFS(Node* startNode, std::function<void(Node * n)> process)
	{
		std::vector<Node* > stack;
		std::vector<Node* > visited;

		stack.push_back(startNode);

		int bre = 0;
		while (!stack.empty())
		{
			Node* node = stack.back();
			stack.pop_back();
			visited.push_back(node);

			process(node);

			for (auto const& edge : node->connections)
			{
				//Add
				if (std::find(stack.begin(), stack.end(), edge.to) != stack.end() || std::find(visited.begin(), visited.end(), edge.to) != visited.end())
				{
					continue;
				}
				else
				{
					stack.push_back(edge.to);
				}
			}
		}
	}

	// == == == == == == == == == == == == 
	// ForEachBFS
	// == == == == == == == == == == == == 
	void ForEachBFS(Node* startNode, std::function<void(Node * n)> process)
	{
		std::list<Node* > stack;
		std::vector<Node* > visited;

		stack.push_back(startNode);

		while (!stack.empty())
		{
			Node* node = stack.front();
			stack.pop_front();
			visited.push_back(node);

			process(node);

			for (auto const& edge : node->connections)
			{
				//Add
				if (std::find(stack.begin(), stack.end(), edge.to) != stack.end() || std::find(visited.begin(), visited.end(), edge.to) != visited.end())
				{
					continue;
				}
				else
				{
					stack.push_front(edge.to);
				}

			}
		}
	}

	// == == == == == == == == == == == == 
	// Find Path
	// == == == == == == == == == == == == 
	//bool FindPath(Node* nodeA, Node* nodeB, std::vector<Nodes*>& output)
	//{

	//}

protected:

	std::vector<Node*> m_nodes;

private:
};