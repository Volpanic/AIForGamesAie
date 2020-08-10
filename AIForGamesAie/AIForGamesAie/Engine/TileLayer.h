#pragma once
#include "raylib.h"
#include "Grid.h"

class TileLayer
{
public:
	TileLayer(Texture2D& texture, int worldWidth, int worldHeight);
	~TileLayer();
	void DrawTilesLayer();
	void DrawTile(Vector2 worldPos, int tileIndex);
	void DrawTile(Vector2 worldPos, Vector2 tileIndex);

	int Vector2ToTileIndex(Vector2& gridPosition);

	void SetTile(Vector2 gridPosition, int tileIndex);
	void SetTile(int gridPosition, int tileIndex);
	void SetTile(Vector2 gridPosition, Vector2 tileIndex);
	void SetTile(int gridPosition, Vector2 tileIndex);

	bool IsCollisionLayer() { return m_collisionLayer; };
	void SetCollisionLayer(bool collisionLayer) { m_collisionLayer = collisionLayer; };

protected:

private:
	const int TILE_SIZE = 16;
	Texture2D m_tilesetTexture;
	Grid<int>* m_tileLayerData = nullptr;
	bool m_collisionLayer = false;
	int m_tilesetWidth = 1;
	int m_tilesetHeight = 1;
};

