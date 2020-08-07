#include "ObjectFactory.h"

//GameObjectsd
#include "Actor.h"
#include "DarkBlueShark.h"
#include "PlayerFish.h"

ObjectFactory::ObjectFactory()
{

}

ObjectFactory::~ObjectFactory()
{
}

GameObject* ObjectFactory::CreateGameObject(std::type_index index, LevelState* level)
{	
	//Cannot switch statement, not ints or enums

	if (index == typeid(DarkBlueShark)) return new DarkBlueShark(level);
	if (index == typeid(PlayerFish)) return new PlayerFish(level);

	return nullptr;
}

GameObject* ObjectFactory::CreateGameObject(std::string index, LevelState* level)
{
	if (index == "DarkBlueShark") return new DarkBlueShark(level);
	if (index == "PlayerFish") return new PlayerFish(level);

	return nullptr;
}

void ObjectFactory::GetAllGameObjectTypes(std::vector<std::type_index>& output)
{
	//output.push_back(typeid(Actor));
	output.push_back(typeid(DarkBlueShark));
	output.push_back(typeid(PlayerFish));
}
