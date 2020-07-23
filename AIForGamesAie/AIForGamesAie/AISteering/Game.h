#pragma once
#include "Application.h"

class Game : public Application
{
public:
	using Application::Application;
	virtual ~Game();
	virtual void Create();
};

