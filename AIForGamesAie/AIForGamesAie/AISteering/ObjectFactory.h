#pragma once
#include "GameObjectFactory.h"

class ObjectFactory : public GameObjectFactory
{
public:
	ObjectFactory();
	virtual ~ObjectFactory();

	virtual GameObject* CreateGameObject(std::type_index index, LevelState* level);

	virtual void GetAllGameObjectTypes(std::vector<std::type_index>& output);
};

