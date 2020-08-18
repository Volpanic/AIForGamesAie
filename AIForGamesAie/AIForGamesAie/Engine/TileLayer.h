#pragma once
#include "raylib.h"
#include "Grid.h"
#include <vector>
#include <list>
#include <tinyxml2.h>
#include <string>

class LevelState;

class TileLayer
{
public:
	TileLayer(std::string layerName,const char* tilesetKey,Texture2D& texture, int worldWidth, int worldHeight);
	~TileLayer();
	void DrawTilesLayer(LevelState* level);
	void DrawTile(Vector2 worldPos, int tileIndex);
	void DrawTile(Vector2 worldPos, Vector2 tileIndex);

	int Vector2ToTileIndex(Vector2 gridPosition);
	Rectangle GetTileDrawRect(int tileIndex);

	bool WithinGrid(int pos) { return m_tileLayerData->WithinGrid(pos); };
	bool WithinGrid(int x, int y) { return m_tileLayerData->WithinGrid(x,y); };

	void SetTile(Vector2 gridPosition, int tileIndex);
	void SetTile(int gridPosition, int tileIndex);
	void SetTile(Vector2 gridPosition, Vector2 tileIndex);
	void SetTile(int gridPosition, Vector2 tileIndex);
	void FloodFillTiles(int x, int y, int value, int targetValue, bool autoTile);
	
	int GetSize() { return m_tileLayerData->GetSize(); };

	void UpdateAutoTile(Vector2 gridPosition);
	void SetAutoTile(Vector2 gridPosition, int value);

	void SetAutoTile(int pos, int value);

	int Get(int pos);
	int Get(int x, int y);

	void SetName(const char* newName);
	std::string GetName();

	void Resize(int newWidth, int newHeight);
	void GetSolids(Rectangle boundingBox, Vector2 position,std::list<Rectangle>& output);
	Texture2D& GetTileTexture() { return m_tilesetTexture; };

	bool IsCollisionLayer() { return m_collisionLayer; };
	void SetCollisionLayer(bool collisionLayer) { m_collisionLayer = collisionLayer; };

	void SaveLayer(tinyxml2::XMLDocument& level, tinyxml2::XMLElement* parentElement);

	bool m_visible = true;

protected:

private:

	int AutoTileToTilesetTile(int autoTileValue);

	int CalculateAutoTileBitmask(int x, int y);

	const int TILE_SIZE = 16;
	const char* m_tilesetPath;
	std::string m_layerName = "";
	Texture2D m_tilesetTexture;
	Grid<int>* m_tileLayerData = nullptr;
	bool m_collisionLayer = true;
	int m_tilesetWidth = 1;
	int m_tilesetHeight = 1;
};

