#pragma once
#include "raylib.h"
#include <vector>

enum PathType { Open, Closed, Reverse };

class Path
{
public:
	Path();
	Path(PathType pathType);
	Path(std::vector<Vector2> data, PathType pathType);
	Path(std::vector<Vector2> data);
	~Path();

	PathType& GetPathType() { return m_pathType; };
	void SetPathType(PathType pathType) { m_pathType = pathType; };

	void PathAddNode(Vector2 newPos);
	Vector2 GetPathNodePosition(int nodeNum);
	int GetPathSize() { return (m_positions.size()); };
	void DrawPath();

protected:

private:
	std::vector<Vector2> m_positions;
	PathType m_pathType;
};

