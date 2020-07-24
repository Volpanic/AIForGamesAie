#pragma once
#include "raylib.h"
#include "Grid.h"
#include <algorithm>
#include <list>

class LevelMap
{
public:

	const int TILE_SIZE = 16;

	LevelMap(int width, int height);
	void Set(int x, int y, int value);
	std::list<Rectangle> GetSolids(Rectangle worldRec, Vector2 offset);
	void Draw();
	Vector2 ToGridPos(const Vector2& pos);
	~LevelMap();

private:
	Grid<int>* m_levelGrid;
};
