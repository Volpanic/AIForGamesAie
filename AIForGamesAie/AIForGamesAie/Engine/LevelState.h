#pragma once
#include <map>
#include <list>
#include "Grid.h"
#include <typeindex>
#include "IGameState.h"
#include "GameObject.h"

class Application;

class LevelState : public IGameState
{
public:

	LevelState(Application* app);
	virtual ~LevelState();

	virtual void Load();
	virtual void Unload();

	virtual void Update(float deltaTime);
	virtual void Draw();


protected:
private:
	Application* m_app;

	Camera2D m_camera = {0};
	Grid<int>* m_levelGrid = new Grid<int>(20,12);

	std::map<std::type_index,std::list<GameObject*>> m_objectTracker;
};