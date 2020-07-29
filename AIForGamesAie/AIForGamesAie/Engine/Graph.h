#pragma once
#include <vector>

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
	void AddEdge(Node* nodeA, Node* nodeB, const TEdgeData& data)
	{
		nodeA->connections.push_back({ nodeB,data });
	}

	// == == == == == == == == == == == == 
	// Get Nodes
	// == == == == == == == == == == == == 
	const std::vector<Node*>& GetNodes() { return m_nodes; };

	// == == == == == == == == == == == == 
	// Connect nodes
	// == == == == == == == == == == == == 
	void ConnectNodes(Node* nodeA, Node* nodeB,const TEdgeData& data)
	{
		AddEdge(nodeA, nodeB, data);
		AddEdge(nodeB, nodeA, data);
	}

protected:

	std::vector<Node*> m_nodes;

private:
};