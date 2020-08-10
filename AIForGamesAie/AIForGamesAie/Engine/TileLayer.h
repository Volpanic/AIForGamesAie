#pragma once
#include "raylib.h"
#include "Grid.h"
#include <vector>
#include <list>

class TileLayer
{
public:
	TileLayer(const char* layerName,Texture2D& texture, int worldWidth, int worldHeight);
	~TileLayer();
	void DrawTilesLayer();
	void DrawTile(Vector2 worldPos, int tileIndex);
	void DrawTile(Vector2 worldPos, Vector2 tileIndex);

	int Vector2ToTileIndex(Vector2& gridPosition);
	Rectangle GetTileDrawRect(int tileIndex);

	bool WithinGrid(int pos) { m_tileLayerData->WithinGrid(pos); };
	bool WithinGrid(int x, int y) { m_tileLayerData->WithinGrid(x,y); };

	void SetTile(Vector2 gridPosition, int tileIndex);
	void SetTile(int gridPosition, int tileIndex);
	void SetTile(Vector2 gridPosition, Vector2 tileIndex);
	void SetTile(int gridPosition, Vector2 tileIndex);
	int GetSize() { return m_tileLayerData->GetSize(); };

	void Resize(int newWidth, int newHeight);
	void GetSolids(Rectangle boundingBox, Vector2 position,std::list<Rectangle>& output);

	bool IsCollisionLayer() { return m_collisionLayer; };
	void SetCollisionLayer(bool collisionLayer) { m_collisionLayer = collisionLayer; };

	const char* m_layerName;

protected:

private:
	const int TILE_SIZE = 16;
	Texture2D m_tilesetTexture;
	Grid<int>* m_tileLayerData = nullptr;
	bool m_collisionLayer = false;
	int m_tilesetWidth = 1;
	int m_tilesetHeight = 1;
};

