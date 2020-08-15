#include "ObjectFactory.h"

//GameObjectsd
#include "Actor.h"
#include "DarkBlueShark.h"
#include "PlayerFish.h"
#include "Clam.h"
#include "BrownShark.h"
#include "Orca.h"

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
	if (index == typeid(BrownShark)) return new BrownShark(level);
	if (index == typeid(Orca)) return new Orca(level);
	if (index == typeid(PlayerFish)) return new PlayerFish(level);
	if (index == typeid(Clam)) return new Clam(level);

	return nullptr;
}

GameObject* ObjectFactory::CreateGameObject(std::string index, LevelState* level)
{
	if (index == "DarkBlueShark") return new DarkBlueShark(level);
	if (index == "BrownShark") return new BrownShark(level);
	if (index == "Orca") return new Orca(level);
	if (index == "PlayerFish") return new PlayerFish(level);
	if (index == "Clam") return new Clam(level);

	return nullptr;
}

void ObjectFactory::GetAllGameObjectTypes(std::vector<std::type_index>& output)
{
	//output.push_back(typeid(Actor));
	output.push_back(typeid(DarkBlueShark));
	output.push_back(typeid(BrownShark));
	output.push_back(typeid(Orca));
	output.push_back(typeid(PlayerFish));
	output.push_back(typeid(Clam));
}
