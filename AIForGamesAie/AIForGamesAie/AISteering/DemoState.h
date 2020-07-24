#pragma once
#include "LevelState.h"
#include "Application.h"
#include "Agent.h"

class DemoState : public LevelState
{
public:
	DemoState(Application* application);
	virtual ~DemoState();
	virtual void Update(float deltaTime);
	virtual void Draw();

private:
	Application* m_app;
	Vector2 m_targetPos = { 0,0 };
};

