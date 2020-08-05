#include "ObjectFactory.h"

//GameObjectsd
#include "Actor.h"

ObjectFactory::ObjectFactory()
{

}

ObjectFactory::~ObjectFactory()
{
}

GameObject* ObjectFactory::CreateGameObject(std::type_index index, LevelState* level)
{	
	//Cannot switch statement, not ints or enums

	if (index == typeid(Actor)) return new Actor(level);

	return nullptr;
}

void ObjectFactory::GetAllGameObjectTypes(std::vector<std::type_index>& output)
{
	output.push_back(typeid(Actor));
}
