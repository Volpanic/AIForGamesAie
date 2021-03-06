#pragma once
#pragma once

template<typename T>
class Grid
{
public:
	Grid(int width, int height);
	Grid(int width, int height,T emptyValue);
	~Grid();

	void Set(int xPos, int yPos, T value);
	void Set(int pos, T value);
	T& Get(int xPos, int yPos);
	T& Get(int pos);
	int GetSize();
	bool WithinGrid(int pos);
	bool WithinGrid(int xPos, int yPos);
	int PositionToInt(int xPos, int yPos);
	int IntToPositionX(int pos);
	int IntToPositionY(int pos);

	int GetWidth() { return m_width; };
	int GetHeight() { return m_height; };

	T* operator[](int index);

private:
	int m_width = 0;
	int m_height = 0;
	T* m_gridArray;

	

};

template<typename T>
Grid<T>::Grid(int width, int height)
{
	m_width = width;
	m_height = height;
	auto size = width * height;
	m_gridArray = new T[size];
}

template<typename T>
Grid<T>::Grid(int width, int height,T emptyValue)
{
	m_width = width;
	m_height = height;
	auto size = width * height;
	m_gridArray = new T[size];

	for (int i = 0; i < size; i++)
	{
		m_gridArray[i] = emptyValue;
	}
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
void Grid<T>::Set(int pos, T value)
{
	if (pos < m_width * m_height)
	{
		m_gridArray[pos] = value;
	}
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
T& Grid<T>::Get(int pos)
{
	if (pos < m_width * m_height)
	{
		return m_gridArray[pos];
	}
	return m_gridArray[0];
}

template<typename T>
int Grid<T>::GetSize()
{
	return m_width * m_height;
}

template<typename T>
int Grid<T>::PositionToInt(int xPos, int yPos)
{
	return ((yPos * m_width) + xPos);
}

template<typename T>
inline int Grid<T>::IntToPositionX(int pos)
{
	return pos % m_width;
}

template<typename T>
inline int Grid<T>::IntToPositionY(int pos)
{
	return pos / m_width;
}

template<typename T>
T* Grid<T>::operator[](int index)
{
	return m_gridArray(index);
}

template<typename T>
bool Grid<T>::WithinGrid(int pos)
{
	if (pos < m_width * m_height)
	{
		return true;
	}
	return false;
}

template<typename T>
bool Grid<T>::WithinGrid(int xPos, int yPos)
{
	if((xPos >= 0 && xPos < GetWidth()) && (yPos >= 0 && yPos < GetHeight()))
	{
		return true;
	}
	return false;
}