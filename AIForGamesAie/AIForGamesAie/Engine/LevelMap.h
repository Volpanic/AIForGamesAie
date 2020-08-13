#pragma once
#include "raylib.h"
#include "Grid.h"
#include <algorithm>
#include <list>
#include <vector>
#include "tinyxml2.h"
#include "Application.h"
#include "TileLayer.h"

class LevelMap
{
public:

	const int TILE_SIZE = 16;

	LevelMap(int width, int height, Application* app);
	void FloodFillTiles(int x, int y, int layer, int value, int targetValue, bool autoTile);
	void Set(int layer,int x, int y, int value);
	void Set(int layer,int pos, int value);
	void SetAutoTile(int layer, int x, int y, int value);
	void SetAutoTile(int layer, int pos, int value);
	bool WithinGrid(int pos);
	int Get(int layer,int pos);
	int Get(int layer,int x, int y);
	bool WithinGrid(int xPos, int yPos);

	void SaveMap(tinyxml2::XMLDocument& level, tinyxml2::XMLElement* parentElement);
	void LoadMap(tinyxml2::XMLDocument& level, tinyxml2::XMLElement* parentElement, Application* app);

	void AddTileLayer(TileLayer newLayer) { m_tileLayers.push_back(newLayer); };
	TileLayer& GetTileLayer(int layer) { return m_tileLayers[layer];};
	int GetTileLayerAmount() { return m_tileLayers.size(); };
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
