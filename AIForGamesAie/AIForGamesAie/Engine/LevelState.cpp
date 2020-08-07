#include "LevelState.h"
#include "raylib.h"
#include "raymath.h"
#include "Application.h"
#include "GameStateManager.h"
#include <iostream>
#include "GameObject.h"
#include "Actor.h"
#include "GameObject.h"
#include "ObjectTracker.h"


LevelState::LevelState(Application* app) : IGameState::IGameState(app)
{
	m_objectTracker = new ObjectTracker();
	m_camera.zoom = 1;
}

LevelState::~LevelState()
{
	delete m_levelMap;
	delete m_objectTracker;
}

void LevelState::Load()
{

}

void LevelState::Unload()
{

}

void LevelState::Update(float deltaTime)
{
	m_objectTracker->Update(deltaTime);
}

void LevelState::Draw()
{

	ClearBackground(WHITE);
	BeginMode2D(m_camera);

	m_levelMap->Draw();
	m_objectTracker->Draw();

	EndMode2D();
}

void LevelState::SetGraph(Graph2D* newGraph)
{
	if (m_graph != nullptr)
	{
		delete m_graph;
	}
	m_graph = newGraph;
}

void LevelState::SetMap(LevelMap* newMap)
{
	if (m_levelMap != nullptr)
	{
		delete m_levelMap;
	}
	m_levelMap = newMap;
}

