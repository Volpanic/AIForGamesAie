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

protected:

	std::vector<Node*> m_nodes;

private:
};