#pragma once
#pragma once

template<typename T>
class Grid
{
public:
	Grid(int width, int height);
	~Grid();

	void Set(int xPos, int yPos, T value);
	T& Get(int xPos, int yPos);

	int GetWidth() { return m_width; };
	int GetHeight() { return m_height; };

private:
	int m_width = 0;
	int m_height = 0;
	T* m_gridArray;

	int PositionToInt(int xPos, int yPos);

};

template<typename T>
Grid<T>::Grid(int width, int height)
{
	m_width = width;
	m_height = height;
	m_gridArray = new T[m_width * m_height];
}

template<typename T>
Grid<T>::~Grid()
{
	
}

template<typename T>
void Grid<T>::Set(int xPos, int yPos, T value)
{
	m_gridArray[PositionToInt(xPos, yPos)] = value;
}

template<typename T>
T& Grid<T>::Get(int xPos, int yPos)
{
	int x = xPos;
	int y = yPos;
	if (xPos < 0) { x = 0;};
	if (yPos < 0) { y = 0;};
	if (xPos > m_width) { x = m_width; };
	if (yPos > m_height) { y = m_height; };

	return m_gridArray[PositionToInt(x, y)];
}

template<typename T>
int Grid<T>::PositionToInt(int xPos, int yPos)
{
	return ((yPos * m_width) + xPos);
}