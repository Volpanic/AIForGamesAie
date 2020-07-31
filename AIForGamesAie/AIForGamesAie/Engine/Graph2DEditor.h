#pragma once
#include "raylib.h"
#include "Graph2D.h"
#include "Path.h"
#include "LevelState.h"

class Graph2DEditor
{
public:
	Graph2DEditor();
	Graph2DEditor(LevelState* levelState);
	~Graph2DEditor();

	void Update(Vector2 mousePos,float deltaTime);
	void Draw();
	void DrawOnlyNodes();

	Graph2D* GetGraph();
	void SetGrapth(Graph2D* graph);

	Path* m_path = nullptr;
	Graph2D::Node* m_selectedNode = nullptr;

protected:
	Graph2D* m_graph =  nullptr;
	Vector2 m_mousePos = {0,0};

	LevelState* m_level = nullptr;

	Graph2D::Node* m_hoverOnNode = nullptr;
	
	Graph2D::Node* m_targetNode = nullptr;
	bool m_connectingNodes = false;

	float m_nodeRadius = 4;
	Color m_nodeCol = { 190,60,99,255 };
	Color m_nodeOutlineCol = { 100,22,57,255 };
	Color m_nodeLineCol = {255,179,179,255};

	float m_nearbyRadius = 32;
	

private:
};

