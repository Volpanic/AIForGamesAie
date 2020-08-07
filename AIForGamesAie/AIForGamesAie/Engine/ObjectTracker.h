#pragma once
#include "GameObject.h"
#include <vector>
#include <list>
#include <typeindex>
#include <map>
#include <functional>

class ObjectTracker
{
public:

	ObjectTracker();
	~ObjectTracker();

	void Update(float deltaTime);
	void Draw();

	template<typename T>
	T* Add(T* gameObject);

	GameObject* Add(std::type_index index, GameObject* gameObject);

	template<typename T>
	void Remove(T* gameObject);

	template<typename T>
	int Count();

	template<typename T>
	std::list<T*> GetAll();

	template<typename T>
	std::list<T*> GetAllRect(Rectangle rect);

	template<typename T>
	T* GetNearest(GameObject* self, const Vector2& position);

	template<typename T>
	bool Exists(T* gameObject);
protected:
	
private:
	std::map<std::type_index, std::list<GameObject*>>* m_objectTracker;
	std::vector<std::function<void()>>* m_commands;
};


//Template defs, breaks if not in header sorry.
template<typename T>
T* ObjectTracker::Add(T* gameObject)
{
	m_commands.push_back([=]()
		{
			(*m_objectTracker)[gameObject->GetCategory()].push_back(gameObject);
		});

	return gameObject;
}

template<typename T>
void ObjectTracker::Remove(T* gameObject)
{
	m_commands.push_back([=]()
		{
			(*m_objectTracker)[gameObject->GetCategory()].remove(gameObject);
			delete gameObject;
		});
}

template<typename T>
int ObjectTracker::Count()
{
	if (m_objectTracker->find(typeid(T)) == m_objectTracker->end())
	{
		//No Entry
		return -1;
	}

	return (*m_objectTracker)[typeid(T)].size();
}

template<typename T>
std::list<T*> ObjectTracker::GetAll()
{
	std::list<T*> newList;

	for (auto const& oldItm : (*m_objectTracker)[typeid(T)])
	{
		newList.push_back(dynamic_cast<T*>(oldItm));
	}

	return newList;
}

template<typename T>
std::list<T*> ObjectTracker::GetAllRect(Rectangle rect)
{
	std::list<T*> newList;

	for (auto const& oldItm : (*m_objectTracker)[typeid(T)])
	{
		if (oldItm->InRectangle(rect))
		{
			newList.push_back(dynamic_cast<T*>(oldItm));
		}
	}

	return newList;
}

template<typename T>
T* ObjectTracker::GetNearest(GameObject* self, const Vector2& position)
{
	GameObject* nearest = nullptr;
	int dist = 0;

	for (auto const& oldItm : (*m_objectTracker)[self->GetCategory()])
	{
		if (oldItm == self)
		{
			continue;
		}

		if (nearest == nullptr)
		{
			nearest = oldItm;
			dist = Vector2Distance(position, oldItm->GetPosition());
		}
		else
		{
			if (Vector2Distance(position, oldItm->GetPosition()) < dist)
			{
				nearest = oldItm;
				dist = Vector2Distance(position, oldItm->GetPosition());
			}
		}
	}

	return dynamic_cast<T*>(nearest);
}

template<typename T>
bool ObjectTracker::Exists(T* gameObject)
{
	if (m_objectTracker->find(gameObject->GetCategory()) == m_objectTracker->end())
	{
		return false;
	}

	for (auto const& oldItm : (*m_objectTracker)[gameObject->GetCategory()])
	{
		if (oldItm == gameObject)
		{
			return true;
		}
	}

	return false;
}