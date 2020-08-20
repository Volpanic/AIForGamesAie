#pragma once
#include "LevelState.h"
#include "Application.h"
#include "Agent.h"
#include "Path.h"
#include <string>

class DemoState : public LevelState
{
public:
	DemoState(Application* app);
	DemoState(Application* app, std::string loadFileName, GameObjectFactory* factory);
	virtual ~DemoState();
	virtual void Update(float deltaTime);
	virtual void Draw();

private:
	Path m_path =  Path(PathType::Closed);

};

