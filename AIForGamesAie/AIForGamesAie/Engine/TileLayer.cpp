#include "TileLayer.h"
#include <algorithm>
#include <string>
#include "LevelState.h"

TileLayer::TileLayer(std::string layerName,const char* tilesetKey,Texture2D& texture, int worldWidth, int worldHeight)
{
	m_layerName = layerName;
	m_tilesetTexture = texture;
	m_tilesetPath = tilesetKey;
	m_tilesetWidth = ceil(texture.width / TILE_SIZE);
	m_tilesetHeight = ceil(texture.height / TILE_SIZE);
	
	m_tileLayerData = new Grid<int>(worldWidth, worldHeight,0);

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
	//delete m_tileLayerData;
}

//Get level data to cull drawing
void TileLayer::DrawTilesLayer(LevelState* level)
{
	if (!m_visible) return;

	Rectangle rect = level->GetCameraRect();

	//GetBounds
	int left =   floor(rect.x / TILE_SIZE);
	int top =    floor(rect.y / TILE_SIZE);
	int right =  ceil((rect.x + rect.width) / TILE_SIZE);
	int bottem = ceil((rect.y + rect.height) / TILE_SIZE);

	if (left < 0) left = 0;
	if (top < 0) top = 0;
	if (right > m_tileLayerData->GetWidth()) right = m_tileLayerData->GetWidth();
	if (bottem > m_tileLayerData->GetHeight()) bottem = m_tileLayerData->GetHeight();

	for (int xx = left; xx < right; xx++)
	{
		for (int yy = top; yy < bottem; yy++)
		{
			if (m_tileLayerData != nullptr)
			{
				DrawTile({(float)(xx * TILE_SIZE),(float)(yy * TILE_SIZE)}, m_tileLayerData->Get(xx, yy));
			}
		}
	}
}

void TileLayer::DrawTile(Vector2 worldPos, int tileIndex)
{
	DrawTexturePro(m_tilesetTexture, GetTileDrawRect(tileIndex), { worldPos.x,worldPos.y,(float)TILE_SIZE,(float)TILE_SIZE }, {0,0},0,WHITE);
}

void TileLayer::DrawTile(Vector2 worldPos, Vector2 tileIndex)
{
	DrawTile(worldPos,Vector2ToTileIndex(tileIndex));
}


int TileLayer::Vector2ToTileIndex(Vector2 gridPosition)
{
	return (((int)(gridPosition.y) * m_tilesetWidth) + (int)(gridPosition.x));
}

Rectangle TileLayer::GetTileDrawRect(int tileIndex)
{
	int xx = tileIndex % m_tilesetWidth;
	int yy = tileIndex / m_tilesetWidth;

	return {(float)(xx * TILE_SIZE),(float)(yy * TILE_SIZE),(float)(TILE_SIZE),(float)(TILE_SIZE)};
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

void TileLayer::FloodFillTiles(int x, int y, int value, int targetValue, bool autoTile)
{
	if (!WithinGrid(x, y))
	{
		return;
	}

	if (m_tileLayerData->Get(x, y) == value)
	{
		return;
	}

	if (m_tileLayerData->Get(x, y) != targetValue)
	{
		return;
	}

	m_tileLayerData->Set(x, y, value);

	FloodFillTiles(x - 1, y, value, targetValue, autoTile);
	FloodFillTiles(x + 1, y, value, targetValue, autoTile);
	FloodFillTiles(x, y - 1, value, targetValue, autoTile);
	FloodFillTiles(x, y + 1, value, targetValue, autoTile);

	if (autoTile)
	{
		UpdateAutoTile({(float)x,(float)y});
	}
}

int TileLayer::CalculateAutoTileBitmask(int x, int y)
{
	bool north = (!WithinGrid(x, y - 1)) ? true : (Get(x, y - 1) > 0);
	bool south = (!WithinGrid(x, y + 1)) ? true : (Get(x, y + 1) > 0);
	bool east = (!WithinGrid(x + 1, y)) ? true : (Get(x + 1, y) > 0);
	bool west = (!WithinGrid(x - 1, y)) ? true : (Get(x - 1, y) > 0);
	bool northEast = north && east;
	bool northWest = north && west;
	bool southWest = south && west;
	bool southEast = south && east;

	return (1 * northWest) + (2 * north) + (4 * northEast) + (8 * west) +
		(16 * east) + (32 * southWest) + (64 * south) + (128 * southEast);
}

void TileLayer::UpdateAutoTile(Vector2 gridPosition)
{
	int x = (int)gridPosition.x;
	int y = (int)gridPosition.y;

	
	int newIndex = AutoTileToTilesetTile(CalculateAutoTileBitmask(x,y));

	if (newIndex >= 4)
	{
		SetTile(gridPosition,newIndex);
	}
}

void TileLayer::SetAutoTile(Vector2 gridPosition,int value)
{
	SetTile(gridPosition, value);
	int xl = (int)gridPosition.x - 1;
	int yl = (int)gridPosition.y - 1;

	for (int xx = xl; xx <= xl + 2; xx++)
	{
		for (int yy = yl; yy <= yl + 2; yy++)
		{
			//if (xx == xl + 1 && yy == yl + 1)
			//{
			//	continue;
			//}

			if (WithinGrid(xx, yy) && Get(xx,yy) > 0)
			{
				UpdateAutoTile({(float)xx,(float)yy});
			}
		}
	}
}

void TileLayer::SetAutoTile(int pos, int value)
{
	int xx = m_tileLayerData->IntToPositionX(pos);
	int yy = m_tileLayerData->IntToPositionY(pos);

	SetTile({(float)xx,(float)yy}, value);
	int xl = xx - 1;
	int yl = yy - 1;

	for (int xx = xl; xx <= xl + 2; xx++)
	{
		for (int yy = yl; yy <= yl + 2; yy++)
		{
			//if (xx == xl + 1 && yy == yl + 1)
			//{
			//	continue;
			//}

			if (WithinGrid(xx, yy) && Get(xx, yy) > 0)
			{
				UpdateAutoTile({ (float)xx,(float)yy });
			}
		}
	}
}

int TileLayer::Get(int pos)
{
	return m_tileLayerData->Get(pos);
}

int TileLayer::Get(int x, int y)
{
	return m_tileLayerData->Get(x,y);
}

void TileLayer::SetName(const char* newName)
{
	m_layerName = newName;
}

std::string TileLayer::GetName()
{
	return m_layerName;
}

void TileLayer::Resize(int newWidth, int newHeight)
{
	Grid<int>* newlevelGrid = new Grid<int>(newWidth, newHeight,0);

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

void TileLayer::SaveLayer(tinyxml2::XMLDocument& level, tinyxml2::XMLElement* parentElement)
{
	auto tileLayer = level.NewElement("TileLayer");

	tileLayer->SetAttribute("LayerName", m_layerName.c_str());
	tileLayer->SetAttribute("TilesetTexture", m_tilesetPath.c_str());
	tileLayer->SetAttribute("LayerWidth",m_tileLayerData->GetWidth());
	tileLayer->SetAttribute("LayerHeight", m_tileLayerData->GetHeight());

	tileLayer->SetAttribute("LayerVisible", m_visible);
	tileLayer->SetAttribute("CollisionLayer", m_collisionLayer);

	tinyxml2::XMLElement* tileData = level.NewElement("TileData");

	//Write tile data
	for (int i = 0; i < m_tileLayerData->GetSize(); i++)
	{
		tinyxml2::XMLElement* gridDataElement = level.NewElement("Tile");
		gridDataElement->SetAttribute("TileValue", m_tileLayerData->Get(i));

		tileData->InsertEndChild(gridDataElement);
	}

	tileLayer->InsertEndChild(tileData);
	parentElement->InsertEndChild(tileLayer);
}

int TileLayer::AutoTileToTilesetTile(int autoTileValue)
{
	//Un-godly
	int x = 0;
	int y = 0;

	switch (autoTileValue)
	{
		//case 41: {x = 2; y = 4; break; }

		case 2:  {x = 3; y = 3; break; }
		case 8:  {x = 2; y = 4; break; }
		case 10: {x = 7; y = 3; break; }
		case 11: {x = 2; y = 3; break; }			  	 
		case 16: {x = 0; y = 4; break; }
		case 18: {x = 4; y = 3; break; }			  		 
		case 22: {x = 0; y = 3; break; }
		case 24: {x = 1; y = 4; break; }
		case 26: {x = 8; y = 3; break; } // Line 1 Complete 

		case 27: {x = 5; y = 3; break; }
		case 30: {x = 6; y = 3; break; }
		case 31: {x = 1; y = 3; break; }
		case 64: {x = 3; y = 1; break; }
		case 66: {x = 3; y = 2; break; }
		case 72: {x = 7; y = 0; break; }
		case 74: {x = 7; y = 4; break; }
		case 75: {x = 7; y = 1; break; } // Line 2 Complete

		case 80: {x = 4; y = 0; break; }
		case 82: {x = 4; y = 4; break; }
		case 86: {x = 4; y = 1; break; }
		case 88: {x = 8; y = 0; break; }//--/--
		case 90: {x = 1; y = 2; break; }
		//case 90: {x = 8; y = 4; break; }
		case 91: {x = 10; y = 3; break; }
		case 94: {x = 9; y = 3; break; }
		case 95: {x = 8; y = 1; break; } // Line 3 Complete //Half
		
		case 104: {x = 2; y = 1; break; }
		case 106: {x = 7; y = 2; break; }
		case 107: {x = 2; y = 2; break; }
		case 120: {x = 5; y = 0; break; }
		case 122: {x = 10; y = 2; break; }
		case 123: {x = 5; y = 4; break; }
		case 126: {x = 9; y = 1; break; } // Line 4 Complete

		case 127: {x = 5; y = 1; break; }
		case 208: {x = 0; y = 1; break; }
		case 210: {x = 4; y = 2; break; }
		case 214: {x = 0; y = 2; break; }
		case 216: {x = 6; y = 0; break; }
		case 218: {x = 9; y = 2; break; }
		case 219: {x = 9; y = 0; break; } // Line 5 Complete

		case 222: {x = 6; y = 4; break; }
		case 223: {x = 6; y = 1; break; }
		case 248: {x = 1; y = 1; break; }
		case 250: {x = 8; y = 2; break; }
		case 251: {x = 5; y = 2; break; }
		case 254: {x = 6; y = 2; break; }
		case 255: {x = 1; y = 2; break; } // Line 6 Complete

		case 0: {x = 3; y = 4; break; }
	}

	return Vector2ToTileIndex({(float)x,(float)y});
}
