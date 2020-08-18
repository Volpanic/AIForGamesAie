#pragma once
#include "Application.h"

class Game : public Application
{
public:
	Game(int gameWidth, int gameHeight, int gameZoom);
	virtual ~Game();
	virtual void Create();

	virtual void StartDraw();
	virtual void EndDraw();

private:
	

	//bool m_Editor = true;
};

