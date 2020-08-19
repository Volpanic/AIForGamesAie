#pragma once
#include "LevelState.h"

class UnderwaterLevelState : public LevelState
{
public:
	UnderwaterLevelState(Application* app);
	UnderwaterLevelState(Application* app, std::string loadFileName, GameObjectFactory* factory);
	virtual ~UnderwaterLevelState();

	virtual void Load();
	virtual void Unload();

	virtual void Update(float deltaTime);
	virtual void Draw();

private:
	bool m_finished = false;
	bool m_hasInputed = false;
};

