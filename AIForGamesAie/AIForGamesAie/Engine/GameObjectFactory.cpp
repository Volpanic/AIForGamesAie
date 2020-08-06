#include "GameObjectFactory.h"
#include "LevelState.h"

GameObjectFactory::GameObjectFactory()
{

}

GameObjectFactory::~GameObjectFactory()
{

}

GameObject* GameObjectFactory::CreateGameObject(std::type_index index, LevelState* level)
{
	return nullptr;
}

GameObject* GameObjectFactory::CreateGameObject(std::string index, LevelState* level)
{
	return nullptr;
}

void GameObjectFactory::GetAllGameObjectTypes(std::vector<std::type_index>& output)
{

}
