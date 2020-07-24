#pragma once
#include <map>
#include <list>
#include "LevelMap.h"
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

	std::list<Rectangle> GetSolids(Rectangle worldRec, Vector2 offset) { return m_levelMap.GetSolids(worldRec, offset); };

protected:
	Application* m_app;
	Camera2D m_camera = { 0 };
	LevelMap m_levelMap = LevelMap(20,12);
	std::map<std::type_index, std::list<GameObject*>> m_objectTracker;
private:
	
};