#pragma once
#include "Graph.h"
#include "raylib.h"

class Graph2D : public Graph<Vector2,float>
{
public:
	Graph2D();
	Graph2D(std::vector<Graph2D::Node*> nodes);
	virtual ~Graph2D();

	std::vector<Graph2D::Node*> GetNearbyNodes(Vector2 position, float radius);

protected:
private:
};