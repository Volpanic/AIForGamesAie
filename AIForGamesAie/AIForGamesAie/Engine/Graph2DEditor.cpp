#include "Graph2DEditor.h"
#include "Graph2D.h"
#include "raymath.h"
#include <iostream>
#include <string>
#include "Debug.h"

Graph2DEditor::Graph2DEditor()
{

}

Graph2DEditor::Graph2DEditor(LevelState* levelState) : m_level(levelState)
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
	
	if (!m_connectingNodes && IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
	{
		if (m_hoverOnNode != nullptr)
		{
			if (m_hoverOnNode == m_selectedNode)
			{
				m_selectedNode = nullptr;
			}
			m_graph->RemoveNode(m_hoverOnNode);
			m_hoverOnNode = nullptr;
		}
	}

	//Conncet
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		if (!m_connectingNodes && m_selectedNode != nullptr)
		{
			if (CheckCollisionPointCircle(m_mousePos, Vector2Add(m_selectedNode->data, { m_nodeRadius * 2,0 }), ceil(m_nodeRadius / 2)))
			{
				m_connectingNodes = true;
			}
		}

		if(m_hoverOnNode == nullptr && !m_connectingNodes)
		{
			m_selectedNode = nullptr;
		}
		else if(m_hoverOnNode != m_selectedNode)
		{
			m_selectedNode = m_hoverOnNode;
		}
	}

	if (IsMouseButtonUp(MOUSE_LEFT_BUTTON))
	{
		if (m_connectingNodes)
		{
			if (m_hoverOnNode != m_selectedNode && m_hoverOnNode != nullptr)
			{
				if (m_selectedNode != nullptr)
				{
					m_graph->ConnectNodes(m_selectedNode, m_hoverOnNode, Vector2Distance(m_selectedNode->data, m_hoverOnNode->data));
				}
			}
			m_connectingNodes = false;
		}
	}

	//Select secondary node
	if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
	{
		if (m_hoverOnNode != nullptr && m_hoverOnNode != m_selectedNode)
		{
			m_targetNode = m_hoverOnNode;
		}
		else
		{
			m_targetNode = nullptr;
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
			DrawLineV(node->data, connection.to->data, m_nodeLineCol);
		}
	}

	//Draw Nodes
	for (auto node : m_graph->GetNodes())
	{
		if (node == m_hoverOnNode)
		{
			DrawCircleV(node->data, m_nodeRadius+2, BLACK);
		}

		if (node != m_selectedNode)
		{
			DrawCircleV(node->data, m_nodeRadius + 1, m_nodeOutlineCol);
			DrawCircleV(node->data, m_nodeRadius, m_nodeCol);
		}
		else
		{
			DrawCircleV(node->data, m_nodeRadius + 1.0f, m_nodeCol);
			DrawCircleV(node->data, m_nodeRadius, m_nodeOutlineCol);

			//Draw Handle
			DrawCircleV(Vector2Add(node->data, { m_nodeRadius * 2,0 }), ceil(m_nodeRadius / 2), m_nodeOutlineCol);
		}

		//Draw Depth Search
		if (m_selectedNode != nullptr)
		{
			m_graph->ForEachDFS(m_selectedNode, [](Graph2D::Node* nde) {
				DrawCircleV(nde->data, 2, SKYBLUE);
				});
		}

		//DrawTargetNode
		if (m_targetNode != nullptr)
		{
			DrawCircleV(m_targetNode->data, 2, BLUE);

			if (m_selectedNode != nullptr)
			{
				std::vector<Graph2D::Node*> nodes;

				Debug::TimeProcess("Dijkstra", [&]() {
					nodes = m_graph->ForEachDijkstra(m_selectedNode, m_targetNode, NULL);
				});

				nodes.clear();

				Debug::TimeProcess("AStar", [&](){
				nodes = m_graph->ForEachAStar(m_selectedNode,m_targetNode,NULL);
				});

				if (m_path != nullptr) delete m_path;
				m_path = new Path();

				for (auto const node : nodes)
				{
					m_path->SetPathType(PathType::Open);
					m_path->PathAddNode(node->data);
				}
				m_targetNode = nullptr;

				//if (m_level != nullptr)
				//{
				//	m_level->Add<>
				//}
			}
		}

		if (m_path != nullptr)
		{
			m_path->DrawPath();
		}
		
	}

	//Draw Preview
	if (m_hoverOnNode == nullptr && m_selectedNode == nullptr)
	{
		DrawCircleV(m_mousePos, m_nodeRadius, m_nodeCol);

		for (auto nearby : m_graph->GetNearbyNodes(m_mousePos, m_nearbyRadius))
		{
			DrawLineV(m_mousePos, nearby->data, LIGHTGRAY);
		}

		if (IsKeyDown(KEY_LEFT_CONTROL))
		{
			DrawCircleV(m_mousePos, m_nearbyRadius, { GRAY.r,GRAY.g,GRAY.g,64 });
		}
	}

	if (m_connectingNodes)
	{
		if (m_hoverOnNode != m_selectedNode)
		{
			if (m_selectedNode != nullptr)
			{
				DrawLineV(m_selectedNode->data, m_mousePos, m_nodeOutlineCol);
			}
		}
		else if(m_hoverOnNode != nullptr)
		{
			
			DrawLineV(m_selectedNode->data, m_hoverOnNode->data, m_nodeOutlineCol);
		}
	}
}

void Graph2DEditor::DrawOnlyNodes()
{
	for (auto node : m_graph->GetNodes())
	{
		for (auto connection : node->connections)
		{
			DrawLineV(node->data, connection.to->data, RAYWHITE);
		}
	}

	//Draw Nodes
	for (auto node : m_graph->GetNodes())
	{
		DrawCircleV(node->data, m_nodeRadius + 1, WHITE);
		DrawCircleV(node->data, m_nodeRadius, RAYWHITE);
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
