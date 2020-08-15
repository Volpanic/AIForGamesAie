#pragma once
#include "Application.h"

struct ImDrawData;

class Game : public Application
{
public:
	Game(int gameWidth, int gameHeight, int gameZoom);
	virtual ~Game();
	virtual void Create();

	virtual void StartDraw();
	virtual void EndDraw();

private:
	ImDrawData* m_drawData = nullptr;

	bool m_Editor = false;
};

