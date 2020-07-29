#pragma once
#include "raylib.h"
#include "Graph2D.h"

class Graph2DEditor
{
public:
	Graph2DEditor();
	~Graph2DEditor();

	void Update(Vector2 mousePos,float deltaTime);
	void Draw();

	Graph2D* GetGraph();
	void SetGrapth(Graph2D* graph);

protected:
	Graph2D* m_graph;
	Vector2 m_mousePos = {0,0};

	Graph2D::Node* m_hoverOnNode = nullptr;
	Graph2D::Node* m_selectedNode = nullptr;

	float m_nodeRadius = 4;
	Color m_nodeCol = { 190,60,99,255 };
	Color m_nodeOutlineCol = { 100,22,57,255 };
	Color m_nodeLineCol = {255,179,179,255};

	float m_nearbyRadius = 32;

private:
};

