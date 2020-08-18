#pragma once
#include "IGameState.h"
#include <string>

class Application;

class MenuState : public IGameState
{
public:

	MenuState(Application* app);
	virtual ~MenuState();

	virtual void Load();
	virtual void Unload();

	virtual void Update(float deltaTime);
	virtual void Draw();


protected:
private:
	std::string m_menuOpts[4] = { "Play Game","Pathfinding Demo","Editor","Exit"};
	const int m_menuOptsSize = 4;
	int m_menuPos = 0;
	float m_timer = 0;

	float m_selectionAlpha = 0;
};