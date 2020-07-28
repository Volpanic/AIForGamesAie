#pragma once
#include "LevelState.h"
#include "Application.h"
#include "Agent.h"
#include "Path.h"

class DemoState : public LevelState
{
public:
	DemoState(Application* app);
	virtual ~DemoState();
	virtual void Update(float deltaTime);
	virtual void Draw();

private:
	Path m_path =  Path(PathType::Reverse);

};

