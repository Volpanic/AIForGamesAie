#include "Path.h"

Path::Path()
{
	m_pathType = PathType::Open;
}

Path::Path(std::vector<Vector2> data)
{
	m_pathType = PathType::Open;
	m_positions = data;
}

Path::Path(PathType pathType)
{
	m_pathType = pathType;
}

Path::Path(std::vector<Vector2> data, PathType pathType)
{
	m_pathType = pathType;
	m_positions = data;
}

void Path::PathAddNode(Vector2 newPos)
{
	m_positions.push_back(newPos);
}

Path::~Path()
{

}

Vector2 Path::GetPathNodePosition(int nodeNum)
{
	return (m_positions[nodeNum]);
}


void Path::DrawPath()
{
	if (m_positions.empty()) return;

	switch (m_pathType)
	{
	case PathType::Open:
	{
		//DrawFirst
		DrawCircleV(m_positions[0], 4, DARKGREEN);
		DrawCircleV(m_positions[0], 2, GREEN);

		//DrawPath
		for (int i = 0; i < m_positions.size() - 1; i++)
		{
			DrawLineEx(m_positions[i], m_positions[i + 1], 2, DARKGRAY);
			DrawCircleV(m_positions[i + 1], 4, DARKGRAY);
			DrawCircleV(m_positions[i + 1], 2, LIGHTGRAY);
		}
		break;
	}

	case PathType::Closed:
	{
		//Draw Path
		int i;
		for (i = 0; i < m_positions.size() - 1; i++)
		{
			DrawLineEx(m_positions[i], m_positions[i + 1], 2, DARKGRAY);
			DrawCircleV(m_positions[i + 1], 4, DARKGRAY);
			DrawCircleV(m_positions[i + 1], 2, LIGHTGRAY);
		}
		//Draw Last to first
		DrawLineEx(m_positions[i], m_positions[0], 2, DARKGRAY);
		DrawCircleV(m_positions[0], 4, DARKGREEN);
		DrawCircleV(m_positions[0], 2, GREEN);

		break;
	}

	case PathType::Reverse:
	{
		//DrawFirst
		DrawCircleV(m_positions[0], 4, DARKGREEN);
		DrawCircleV(m_positions[0], 2, GREEN);

		//DrawPath
		for (int i = 0; i < m_positions.size() - 1; i++)
		{
			DrawLineEx(m_positions[i], m_positions[i + 1], 2, DARKGRAY);
			DrawCircleV(m_positions[i + 1], 4, DARKGRAY);
			DrawCircleV(m_positions[i + 1], 2, LIGHTGRAY);
		}
		break;
	}
	}
}