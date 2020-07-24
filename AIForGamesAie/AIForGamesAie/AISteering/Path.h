#pragma once
#include "raylib.h"
#include <vector>

class Path
{
public:
	Path() {};
	Path(std::vector<Vector2> data);
	~Path();

	void PathAddNode(Vector2 newPos);
	Vector2 GetPathNodePosition(int nodeNum);
	int GetPathSize() { return m_positions.size(); };
	void DrawPath();

protected:

private:
	std::vector<Vector2> m_positions;
};

