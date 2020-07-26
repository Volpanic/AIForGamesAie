#include "LevelState.h"
#include "raylib.h"
#include "raymath.h"
#include "Application.h"
#include "GameStateManager.h"
#include <iostream>
#include "Actor.h"

LevelState::LevelState(Application* app) : m_app(app)
{
	m_camera.zoom = 1;
}

LevelState::~LevelState()
{
	for (auto const& objList : m_objectTracker)
	{
		for (auto obj = objList.second.begin(); obj != objList.second.end(); obj++)
		{
			//delete* obj;
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
	//Invoke chached commands
	for (auto cmd : m_commands)
	{
		cmd();
	}
	m_commands.clear();

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

	EndMode2D();
}

//Template defs
template<typename T>
void LevelState::Add(T* gameObject)
{
	m_commands.push_back([=]()
	{
		m_objectTracker[typeid(T)].push_back(gameObject);
	});
}

template<typename T>
void LevelState::Remove(T* gameObject)
{
	m_commands.push_back([=]()
	{
		m_objectTracker[typeid(T)].remove(gameObject);
		delete gameObject;
	});
}

template<typename T>
int LevelState::Count()
{
	if (m_objectTracker.find(typeid(T)) == m_objectTracker.end())
	{
		//No Entry
		return -1;
	}

	return m_objectTracker[typeid(T)].size();
}

template<typename T>
std::list<T*> LevelState::GetAll()
{
	std::list<T*> newList;

	for (auto const& oldItm : m_objectTracker[typeid(T)])
	{
		newList.push_back(dynamic_cast<T*>(oldItm));
	}

	return newList;
}

template<typename T>
std::list<T*> LevelState::GetAllRect(Rectangle rect)
{
	std::list<T*> newList;

	for (auto const& oldItm : m_objectTracker[typeid(T)])
	{
		if (oldItm->InRectangle(rect))
		{
			newList.push_back(dynamic_cast<T*>(oldItm));
		}
	}

	return newList;
}