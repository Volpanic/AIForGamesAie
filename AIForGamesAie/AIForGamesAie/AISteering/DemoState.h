#pragma once
#include "LevelState.h"
#include "Application.h"
#include "Agent.h"
#include "Path.h"

class DemoState : public LevelState
{
public:
	DemoState(Application* application);
	virtual ~DemoState();
	virtual void Update(float deltaTime);
	virtual void Draw();

private:
	Application* m_app;
	Path m_path =  Path(PathType::Reverse);

};

