#pragma once
#include "raylib.h"
#include "Grid.h"
#include <algorithm>
#include <list>
#include <vector>
#include "TileLayer.h"

class LevelMap
{
public:

	const int TILE_SIZE = 16;

	LevelMap(int width, int height, Application* app);
	void Set(int layer,int x, int y, int value);
	void Set(int layer,int pos, int value);
	bool WithinGrid(int pos);
	bool WithinGrid(int xPos, int yPos);

	int GetSize();

	void Resize(int newWidth, int newHeight);

	std::list<Rectangle> GetSolids(Rectangle worldRec, Vector2 offset);
	void Draw();
	Vector2 ToGridPos(const Vector2& pos);

	int GetWidth() { return m_worldWidth; };
	int GetHeight() { return m_worldHeight; };

	~LevelMap();

private:
	int m_worldWidth = 1;
	int m_worldHeight = 1;
	std::vector<TileLayer> m_tileLayers;
};
