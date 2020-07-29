#include "Graph2DEditor.h"
#include "Graph2D.h"
#include "raymath.h"

Graph2DEditor::Graph2DEditor()
{

}

Graph2DEditor::~Graph2DEditor()
{

}

void Graph2DEditor::Update(Vector2 mousePos,float deltaTime)
{
	m_mousePos = mousePos;
	m_hoverOnNode = nullptr;

	//Expand radius
	if (IsKeyDown(KEY_LEFT_CONTROL))
	{
		m_nearbyRadius += GetMouseWheelMove()*2;
		m_nearbyRadius = Clamp(m_nearbyRadius,1,128);
	}

	//Hover on node
	for (auto node : m_graph->GetNodes())
	{
		if (CheckCollisionCircles(m_mousePos,m_nodeRadius,node->data,m_nodeRadius))
		{
			m_hoverOnNode = node;
		}
	}

	//Add
	if (m_selectedNode == nullptr && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		//Prevent nodes from being ontop of one another
		if (m_hoverOnNode == nullptr)
		{
			Graph2D::Node* newNode = m_graph->AddNode(m_mousePos);

			for (auto nearby : m_graph->GetNearbyNodes(m_mousePos, m_nearbyRadius))
			{
				m_graph->ConnectNodes(newNode, nearby, Vector2Distance(newNode->data, nearby->data));
			}
		}
		else
		{
			m_selectedNode = m_hoverOnNode;
		}
	}
}

void Graph2DEditor::Draw()
{
	//Draw Lines
	for (auto node : m_graph->GetNodes())
	{
		for (auto connection : node->connections)
		{
			DrawLineV(node->data,connection.to->data, m_nodeLineCol);
		}
	}

	//Draw Nodes
	for (auto node : m_graph->GetNodes())
	{
		if (node == m_hoverOnNode)
		{
			DrawCircleV(node->data, m_nodeRadius+2, BLACK);
		}

		DrawCircleV(node->data, m_nodeRadius+1, m_nodeOutlineCol);
		DrawCircleV(node->data, m_nodeRadius, m_nodeCol);
	}

	//Draw Preview
	if (m_hoverOnNode == nullptr && m_selectedNode == nullptr)
	{
		DrawCircleV(m_mousePos, m_nodeRadius, m_nodeCol);

		for (auto nearby : m_graph->GetNearbyNodes(m_mousePos, m_nearbyRadius))
		{
			DrawLineV(m_mousePos, nearby->data, LIGHTGRAY);
		}

		DrawCircleV(m_mousePos, m_nearbyRadius, { GRAY.r,GRAY.g,GRAY.g,64 });
	}
}


Graph2D* Graph2DEditor::GetGraph()
{
	return m_graph;
}

void Graph2DEditor::SetGrapth(Graph2D* graph)
{
	m_graph = graph;
}
