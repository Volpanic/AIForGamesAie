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
	void Set(int pos, int value);
	int Get(int x, int y);
	int Get(int pos);

	int GetSize();

	std::list<Rectangle> GetSolids(Rectangle worldRec, Vector2 offset);
	void Draw();
	Vector2 ToGridPos(const Vector2& pos);

	int GetWidth() { return m_levelGrid->GetWidth(); };
	int GetHeight() { return m_levelGrid->GetHeight(); };

	~LevelMap();

private:
	Grid<int>* m_levelGrid;
};
