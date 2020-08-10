#include "LevelMap.h"
#include "Application.h"
#include "TileLayer.h"

LevelMap::LevelMap(int width, int height, Application* app)
{
	m_worldWidth = width;
	m_worldHeight = height;

	m_tileLayers.push_back(TileLayer("Collision",app->GetResources()->GetTileset("tle_collision"),width,height));

	for (int xx = 0; xx < GetWidth(); xx++)
	{
		for (int yy = 0; yy < GetHeight(); yy++)
		{
			Set(xx, yy, 0);
		}
	}
}

void LevelMap::Set(int layer,int x, int y, int value)
{
	if (m_tileLayers[layer].WithinGrid(x,y))
	{
		m_tileLayers[layer].SetTile({(float)x,(float)y},value);
	}
}

void LevelMap::Set(int layer, int pos, int value)
{
	if (m_tileLayers[layer].WithinGrid(pos))
	{
		m_tileLayers[layer].SetTile(pos, value);
	}
}

bool LevelMap::WithinGrid(int pos)
{
	return m_tileLayers[0].WithinGrid(pos);
}

bool LevelMap::WithinGrid(int xPos, int yPos)
{
	return m_tileLayers[0].WithinGrid(xPos,yPos);
}

int LevelMap::GetSize()
{
	return m_tileLayers[0].GetSize();
}

void LevelMap::Resize(int newWidth, int newHeight)
{
	for (int i = 0; i < m_tileLayers.size(); i++)
	{
		m_tileLayers[i].Resize(newWidth, newHeight);
	}
}

std::list<Rectangle> LevelMap::GetSolids(Rectangle boundingBox, Vector2 position)
{
	std::list<Rectangle> returnList;

	for (int i = 0; i < m_tileLayers.size(); i++)
	{
		m_tileLayers[i].GetSolids(boundingBox,position,returnList);
	}

	return returnList;
}

void LevelMap::Draw()
{
	for (int i = 0; i < m_tileLayers.size(); i++)
	{
		m_tileLayers[i].DrawTilesLayer();
	}
}

Vector2 LevelMap::ToGridPos(const Vector2& pos)
{
	return {(float)floor(pos.x / TILE_SIZE),(float)floor(pos.y / TILE_SIZE) };
}

LevelMap::~LevelMap()
{
	
}