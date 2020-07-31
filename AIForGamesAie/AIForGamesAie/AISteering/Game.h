#pragma once
#include "Application.h"

struct ImDrawData;

class Game : public Application
{
public:
	using Application::Application;
	virtual ~Game();
	virtual void Create();

	virtual void StartDraw();
	virtual void EndDraw();

private:
	ImDrawData* m_drawData;
};

