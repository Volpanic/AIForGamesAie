#pragma once
#include "IGameState.h"

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
	float m_timer = 0.0f;
	Application* m_app;
};