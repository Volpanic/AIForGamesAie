#include "LevelState.h"
#include "raylib.h"
#include "raymath.h"
#include "Application.h"
#include "GameStateManager.h"
#include <iostream>
#include "Actor.h"

LevelState::LevelState(Application* app) : m_app(app)
{
	m_objectTracker[typeid(Actor)].push_back(new Actor());

	m_camera.zoom = 1;
}

LevelState::~LevelState()
{
	delete m_levelGrid;
	for (auto const& objList : m_objectTracker)
	{
		for (auto obj = objList.second.begin(); obj != objList.second.end(); obj++)
		{
			delete (*obj);
		}
	}
}

void LevelState::Load()
{

}

void LevelState::Unload()
{

}

void LevelState::Update(float deltaTime)
{
	//Update Objects
	for (auto const& objList : m_objectTracker)
	{
		for (auto obj = objList.second.begin(); obj != objList.second.end(); obj++)
		{
			(*obj)->Update(deltaTime);
		}
	}
}

void LevelState::Draw()
{
	BeginMode2D(m_camera);

	//Draw Objects
	for (auto const& objList : m_objectTracker)
	{
		for (auto obj = objList.second.begin(); obj != objList.second.end(); obj++)
		{
			(*obj)->Draw();
		}
	}

	for(int x = 0; x < m_levelGrid->GetWidth(); x++)
	{
		for (int y = 0; y < m_levelGrid->GetWidth(); y++)
		{
			if (m_levelGrid->Get(x, y) == 1)
			{
				DrawRectangle(x*16,y*16,16,16,DARKGRAY);
			}
		}
	}

	EndMode2D();
}