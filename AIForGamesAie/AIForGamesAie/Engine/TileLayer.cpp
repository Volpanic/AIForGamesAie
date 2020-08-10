#include "TileLayer.h"
#include <algorithm>

TileLayer::TileLayer(const char* layerName,Texture2D& texture, int worldWidth, int worldHeight)
{
	m_tilesetTexture = texture;
	m_tilesetWidth = texture.width / TILE_SIZE;
	m_tilesetHeight = texture.height / TILE_SIZE;
	
	m_tileLayerData = new Grid<int>(worldWidth, worldHeight);
	m_layerName = layerName;

	for (int xx = 0; xx < m_tileLayerData->GetWidth(); xx++)
	{
		for (int yy = 0; yy < m_tileLayerData->GetHeight(); yy++)
		{
			m_tileLayerData->Set(xx, yy, 0);
		}
	}
}

TileLayer::~TileLayer()
{
	delete m_layerName;
	if (m_tileLayerData != NULL) delete m_tileLayerData;
}

void TileLayer::DrawTilesLayer()
{
	for (int xx = 0; xx < m_tileLayerData->GetWidth(); xx++)
	{
		for (int yy = 0; yy < m_tileLayerData->GetHeight(); yy++)
		{
			DrawTile({(float)(xx * TILE_SIZE),(float)(yy * TILE_SIZE)}, m_tileLayerData->Get(xx, yy));
		}
	}
}

void TileLayer::DrawTile(Vector2 worldPos, int tileIndex)
{
	DrawTexturePro(m_tilesetTexture, GetTileDrawRect(tileIndex), { worldPos.x,worldPos.y,TILE_SIZE,TILE_SIZE }, {0,0},0,WHITE);
}

void TileLayer::DrawTile(Vector2 worldPos, Vector2 tileIndex)
{
	DrawTile(worldPos,Vector2ToTileIndex(tileIndex));
}

int TileLayer::Vector2ToTileIndex(Vector2& gridPosition)
{
	return (((int)(gridPosition.y) * m_tilesetWidth) + (int)(gridPosition.x));
}

Rectangle TileLayer::GetTileDrawRect(int tileIndex)
{
	int xx = tileIndex;
	int yy = 0;

	while (tileIndex > m_tilesetWidth)
	{
		xx -= m_tilesetWidth;
		yy++;
	}

	return {xx * TILE_SIZE,yy * TILE_SIZE,TILE_SIZE,TILE_SIZE};
}

void TileLayer::SetTile(Vector2 gridPosition, int tileIndex)
{
	m_tileLayerData->Set((int)gridPosition.x, (int)gridPosition.y, tileIndex);
}

void TileLayer::SetTile(int gridPosition, int tileIndex)
{
	m_tileLayerData->Set(gridPosition,tileIndex);
}

void TileLayer::SetTile(Vector2 gridPosition, Vector2 tileIndex)
{
	m_tileLayerData->Set((int)gridPosition.x, (int)gridPosition.y, Vector2ToTileIndex(tileIndex));
}

void TileLayer::SetTile(int gridPosition, Vector2 tileIndex)
{
	m_tileLayerData->Set(gridPosition, Vector2ToTileIndex(tileIndex));
}

void TileLayer::Resize(int newWidth, int newHeight)
{
	Grid<int>* newlevelGrid = new Grid<int>(newWidth, newHeight);

	for (int xx = 0; xx < m_tileLayerData->GetWidth() && xx < newWidth; xx++)
	{
		for (int yy = 0; yy < m_tileLayerData->GetHeight() && yy < newHeight; yy++)
		{
			newlevelGrid->Set(xx, yy, m_tileLayerData->Get(xx, yy));
		}
	}

	delete m_tileLayerData;
	m_tileLayerData = newlevelGrid;
}

void TileLayer::GetSolids(Rectangle boundingBox, Vector2 position, std::list<Rectangle>& output)
{
	if (!m_collisionLayer)
	{
		return;
	}

	boundingBox.x = position.x;
	boundingBox.y = position.y;

	//Get min and max possible tiles
	int x1 = floor(boundingBox.x / TILE_SIZE) - 1;
	int x2 = ceil((boundingBox.x + boundingBox.width) / TILE_SIZE) + 1;

	int y1 = floor(boundingBox.y / TILE_SIZE) - 1;
	int y2 = ceil((boundingBox.y + boundingBox.height) / TILE_SIZE) + 1;

	x1 = std::max(x1, 0);
	x2 = std::min(x2, m_tileLayerData->GetWidth());

	y1 = std::max(y1, 0);
	y2 = std::min(y2, m_tileLayerData->GetHeight());

	//Return all solids hitboxes
	for (int xx = x1; xx <= x2; xx++)
	{
		for (int yy = y1; yy <= y2; yy++)
		{
			if (m_tileLayerData->Get(xx, yy) >= 1)
			{
				output.push_back({ (float)(xx * TILE_SIZE),(float)(yy * TILE_SIZE),(float)TILE_SIZE ,(float)TILE_SIZE });
			}
		}
	}
}
