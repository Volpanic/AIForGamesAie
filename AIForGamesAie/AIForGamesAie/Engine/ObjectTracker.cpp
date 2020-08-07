#include "ObjectTracker.h"
#include <vector>
#include <list>
#include <typeindex>
#include <map>
#include <functional>

ObjectTracker::ObjectTracker()
{
	m_objectTracker = new std::map<std::type_index, std::list<GameObject*>>;
	m_commands = new std::vector<std::function<void()>>;
}

ObjectTracker::~ObjectTracker()
{
	for (auto const& objList : (*m_objectTracker))
	{
		for (auto obj = objList.second.begin(); obj != objList.second.end(); obj++)
		{
			delete* obj;
		}
	}
	delete m_objectTracker;
	delete m_commands;
}

GameObject* ObjectTracker::Add(std::type_index index, GameObject* gameObject)
{
	m_commands->push_back([=]()
		{
			(*m_objectTracker)[index].push_back(gameObject);
		});

	return gameObject;
}

void ObjectTracker::Update(float deltaTime)
{
	//Invoke chached commands
	if (!m_commands->empty())
	{
		for (auto cmd : (*m_commands))
		{
			cmd();
		}
		m_commands->clear();
	}
	

	//Update Objects
	for (auto const& objList : (*m_objectTracker))
	{
		for (auto obj = objList.second.begin(); obj != objList.second.end(); obj++)
		{
			(*obj)->Update(deltaTime);
		}
	}
}

void ObjectTracker::Draw()
{
	//Draw Objects
	for (auto const& objList : (*m_objectTracker))
	{
		for (auto obj = objList.second.begin(); obj != objList.second.end(); obj++)
		{
			(*obj)->Draw();
		}
	}
}