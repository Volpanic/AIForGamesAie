#pragma once
#include <list>
#include "LevelMap.h"
#include <typeindex>
#include "IGameState.h"
#include "Application.h"
#include "Graph2D.h"
#include "GameObjectFactory.h"

class ObjectTracker;
class Application;

class LevelState : public IGameState
{
public:

	LevelState(Application* app);
	LevelState(Application* app, std::string loadFileName,GameObjectFactory* factory);
	virtual ~LevelState();

	virtual void Load();
	virtual void Unload();

	virtual void Update(float deltaTime);
	virtual void Draw();

	Vector2 GetScaledMousePos() { return m_app->GetScaledMousePos(); };
	Graph2D* GetGraph() { return m_graph; };
	ObjectTracker* GetObjectTracker() { return m_objectTracker; };

	void SetGraph(Graph2D* newGraph);
	void SetMap(LevelMap* newMap);
	void SetCameraPositoin(Vector2 position) { m_camera.target = position; };

	std::list<Rectangle> GetSolids(Rectangle worldRec, Vector2 offset) { return m_levelMap->GetSolids(worldRec, offset); };

	void SaveMap(std::string fileName);
	bool LoadMap(std::string fileName, GameObjectFactory* factory);

	//Template stubs
	//Tracking

	

protected:
	Camera2D m_camera = { 0 };
	LevelMap* m_levelMap = new LevelMap(20,12);
	ObjectTracker* m_objectTracker;

	std::string m_levelName = "Black";
	Graph2D* m_graph = nullptr;

	float* m_mapClearColour = new float[3];

private:
	
};