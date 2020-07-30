#include "LevelMap.h"

LevelMap::LevelMap(int width, int height)
{
	m_levelGrid = new Grid<int>(width, height);

	for (int xx = 0; xx < GetWidth(); xx++)
	{
		for (int yy = 0; yy < GetHeight(); yy++)
		{
			Set(xx, yy, 0);
		}
	}
}

void LevelMap::Set(int x, int y, int value)
{
	if ((x >= 0 && x < m_levelGrid->GetWidth()) && (y >= 0 && y < m_levelGrid->GetHeight()))
	{
		m_levelGrid->Set(x, y, value);
	}
}

void LevelMap::Set(int pos, int value)
{
	m_levelGrid->Set(pos,value);
}

int LevelMap::Get(int x, int y)
{
	if ((x >= 0 && x < m_levelGrid->GetWidth()) && (y >= 0 && y < m_levelGrid->GetHeight()))
	{
		return m_levelGrid->Get(x,y);
	}
	return 0;
}

int LevelMap::Get(int pos)
{
	return m_levelGrid->Get(pos);
}

int LevelMap::GetSize()
{
	return m_levelGrid->GetSize();
}

std::list<Rectangle> LevelMap::GetSolids(Rectangle boundingBox, Vector2 position)
{
	std::list<Rectangle> returnList;

	boundingBox.x = position.x;
	boundingBox.y = position.y;

	//Get min and max possible tiles
	int x1 = floor(boundingBox.x / TILE_SIZE)-1;
	int x2 = ceil((boundingBox.x + boundingBox.width) / TILE_SIZE)+1;

	int y1 = floor(boundingBox.y / TILE_SIZE)-1;
	int y2 = ceil((boundingBox.y + boundingBox.height) / TILE_SIZE)+1;

	x1 = std::max(x1, 0);
	x2 = std::min(x2, m_levelGrid->GetWidth());

	y1 = std::max(y1, 0);
	y2 = std::min(y2, m_levelGrid->GetHeight());

	//Return all solids hitboxes
	for (int xx = x1; xx <= x2; xx++)
	{
		for (int yy = y1; yy <= y2; yy++)
		{
			if (m_levelGrid->Get(xx, yy) == 1)
			{
				returnList.push_back({ (float)(xx * TILE_SIZE),(float)(yy * TILE_SIZE),(float)TILE_SIZE ,(float)TILE_SIZE });
			}
		}
	}

	return returnList;
}

void LevelMap::Draw()
{
	for (int x = 0; x < m_levelGrid->GetWidth(); x++)
	{
		for (int y = 0; y < m_levelGrid->GetWidth(); y++)
		{
			if (m_levelGrid->Get(x, y) == 1)
			{
				DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKGRAY);
			}
		}
	}
}

Vector2 LevelMap::ToGridPos(const Vector2& pos)
{
	return {(float)floor(pos.x / TILE_SIZE),(float)floor(pos.y / TILE_SIZE) };
}

LevelMap::~LevelMap()
{
	delete m_levelGrid;
}