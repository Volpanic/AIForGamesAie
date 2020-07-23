#pragma once
#include "IGameState.h"
#include "Application.h"
#include "Agent.h"

class DemoState : public IGameState
{
public:
	DemoState(Application* application);
	virtual ~DemoState();
	virtual void Update(float deltaTime);
	virtual void Draw();

private:
	Application* m_app;
	Agent agent;
};

