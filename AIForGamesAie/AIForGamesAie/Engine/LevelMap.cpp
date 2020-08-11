#include "LevelMap.h"
#include "Application.h"
#include "TileLayer.h"

LevelMap::LevelMap(int width, int height, Application* app)
{
	m_worldWidth = width;
	m_worldHeight = height;

	m_tileLayers.push_back(TileLayer("Collision","tle_collision.png",app->GetResources()->GetTileset("tle_collision.png"),width,height));

	for (int i = 0; i < m_tileLayers.size(); i++)
	{
		for (int xx = 0; xx < GetWidth(); xx++)
		{
			for (int yy = 0; yy < GetHeight(); yy++)
			{
				Set(i, xx, yy, 0);
			}
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

void LevelMap::SaveMap(tinyxml2::XMLDocument& level, tinyxml2::XMLElement* parentElement)
{
	for (int i = 0; i < m_tileLayers.size(); i++)
	{
		m_tileLayers[i].SaveLayer(level,parentElement);
	}
}

void LevelMap::LoadMap(tinyxml2::XMLDocument& level, tinyxml2::XMLElement* parentElement,Application* app)
{
	m_tileLayers.clear();
	tinyxml2::XMLElement* tileLayer = parentElement->FirstChildElement("TileLayer");

	while (tileLayer != nullptr)
	{
		const char* layerName = "";
		const char* texturePath = "";
		int width = 1;
		int height = 1;

		bool visible = true;
		bool collisionLayer = false;

		//Get map vars
		tileLayer->QueryAttribute("LayerName", &layerName);
		tileLayer->QueryAttribute("TilesetTexture", &texturePath);
		tileLayer->QueryAttribute("LayerWidth", &width);
		tileLayer->QueryAttribute("LayerHeight", &height);

		tileLayer->QueryAttribute("LayerVisible", &visible);
		tileLayer->QueryAttribute("CollisionLayer", &collisionLayer);

		TileLayer tl = TileLayer(layerName,texturePath,app->GetResources()->GetTileset(texturePath),width,height);
		tl.SetCollisionLayer(collisionLayer);
		tl.m_visible = visible;

		//Load in tile data
		int pos = 0;
		tinyxml2::XMLElement* tileData = tileLayer->FirstChildElement("TileData");
		tinyxml2::XMLElement* tileElement = tileData->FirstChildElement("Tile");

		while (tileElement != nullptr)
		{
			int tileValue = 0;
			tileElement->QueryAttribute("TileValue",&tileValue);

			tl.SetTile(pos,tileValue);

			pos++;
			tileElement = tileElement->NextSiblingElement();
		}

		tileLayer = tileLayer->NextSiblingElement();

		//Put Tile layer in list
		m_tileLayers.push_back(tl);
	}
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

	m_worldWidth = newWidth;
	m_worldHeight = newHeight;
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