#include "UnderwaterLevelState.h"
#include "ObjectTracker.h"
#include "Clam.h"
#include <string>

UnderwaterLevelState::UnderwaterLevelState(Application* app) : LevelState(app)
{

}

UnderwaterLevelState::UnderwaterLevelState(Application* app, std::string loadFileName, GameObjectFactory* factory) : LevelState(app,loadFileName,factory)
{

}

UnderwaterLevelState::~UnderwaterLevelState()
{
}

void UnderwaterLevelState::Load()
{
	LevelState::Load();
}

void UnderwaterLevelState::Unload()
{
	LevelState::Unload();
}

void UnderwaterLevelState::Update(float deltaTime)
{
	LevelState::Update(deltaTime);

	m_camera.target.x = Clamp(m_camera.target.x,0,m_levelMap->GetWidth() * m_levelMap->TILE_SIZE - (m_app->GetGameWidth()));
	m_camera.target.y = Clamp(m_camera.target.y,0, m_levelMap->GetHeight() * m_levelMap->TILE_SIZE - (m_app->GetGameHeight()));
}

void UnderwaterLevelState::Draw()
{
	LevelState::Draw();

	int clamCount = GetObjectTracker()->Count<Clam>([](Clam* object) {
		return object->HasPearl();
	});

	std::string pearlText = ("Pearls x" + std::to_string(clamCount));
	DrawText(pearlText.c_str(), 8+1, 8, 8, BLACK);
	DrawText(pearlText.c_str(), 8-1, 8, 8, BLACK);
	DrawText(pearlText.c_str(), 8, 8+1, 8, BLACK);
	DrawText(pearlText.c_str(), 8, 8-1, 8, BLACK);
	DrawText(pearlText.c_str(), 8, 8, 8, WHITE);
}
