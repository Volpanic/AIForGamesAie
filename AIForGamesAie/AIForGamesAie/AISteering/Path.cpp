#include "Path.h"

Path::Path(std::vector<Vector2> data)
{
	m_positions = data;
}

Path::~Path()
{

}

void Path::PathAddNode(Vector2 newPos)
{
	m_positions.push_back(newPos);
}

Vector2 Path::GetPathNodePosition(int nodeNum)
{
	return (m_positions[nodeNum]);
}


void Path::DrawPath()
{
	if (m_positions.empty()) return;
	for (int i = 0; i < m_positions.size()-1; i++)
	{
		DrawLineEx(m_positions[i], m_positions[i+1],2,DARKGRAY);

		DrawCircleV(m_positions[i+1], 4, DARKGRAY);
		DrawCircleV(m_positions[i+1], 2, LIGHTGRAY);
	}
}