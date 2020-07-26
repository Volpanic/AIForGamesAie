#pragma once
#include <map>
#include <list>
#include "LevelMap.h"
#include <typeindex>
#include "IGameState.h"
#include "GameObject.h"
#include "Application.h"
#include <functional>

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

	Vector2 GetScaledMousePos() { return m_app->GetScaledMousePos(); };

	std::list<Rectangle> GetSolids(Rectangle worldRec, Vector2 offset) { return m_levelMap.GetSolids(worldRec, offset); };

	//Template stubs
	//Tracking
	template<typename T>
	void Add(T* gameObject);

	template<typename T>
	void Remove(T* gameObject);

	template<typename T>
	int Count();

	template<typename T>
	std::list<T*> GetAll();

	template<typename T>
	std::list<T*> GetAllRect(Rectangle rect);
	

protected:
	Application* m_app;
	Camera2D m_camera = { 0 };
	LevelMap m_levelMap = LevelMap(20,12);
	std::map<std::type_index, std::list<GameObject*>> m_objectTracker;

private:
	std::vector<std::function<void()>> m_commands;
};
