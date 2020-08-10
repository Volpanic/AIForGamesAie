#include "TileLayer.h"

TileLayer::TileLayer(Texture2D& texture, int worldWidth, int worldHeight)
{
	m_tilesetTexture = texture;
	m_tilesetWidth = texture.width / TILE_SIZE;
	m_tilesetHeight = texture.height / TILE_SIZE;
	
	m_tileLayerData = new Grid<int>(worldWidth, worldHeight);
}

TileLayer::~TileLayer()
{
	if (m_tileLayerData != NULL) delete m_tileLayerData;
}

void TileLayer::DrawTilesLayer()
{
}

void TileLayer::DrawTile(Vector2 worldPos, int tileIndex)
{
}

void TileLayer::DrawTile(Vector2 worldPos, Vector2 tileIndex)
{
}

int TileLayer::Vector2ToTileIndex(Vector2& gridPosition)
{
	//return m_tileLayerData->PositionToInt((int)gridPosition.x, (int)gridPosition.y);
}

void TileLayer::SetTile(Vector2 gridPosition, int tileIndex)
{
}

void TileLayer::SetTile(int gridPosition, int tileIndex)
{
}

void TileLayer::SetTile(Vector2 gridPosition, Vector2 tileIndex)
{
}

void TileLayer::SetTile(int gridPosition, Vector2 tileIndex)
{
}
