#include "UnderwaterLevelState.h"
#include "ObjectTracker.h"
#include "Clam.h"
#include "GameStateManager.h"
#include "MenuState.h"
#include "FadeTransition.h"
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
	if (!m_finished)
	{
		LevelState::Update(deltaTime);
	}
	else
	{
		if ((!m_hasInputed) && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Z)))
		{
			m_app->GetGameStateManager()->SetState("MainMenu", new MenuState(m_app));
			m_app->GetGameStateManager()->PushState("MainMenu", new FadeTransition(m_app, "MainMenu", 1.0f));
			m_hasInputed = true;
		}
	}

	m_camera.target.x = Clamp(m_camera.target.x,0,m_levelMap->GetWidth() * m_levelMap->TILE_SIZE - (m_app->GetGameWidth()));
	m_camera.target.y = Clamp(m_camera.target.y,0, m_levelMap->GetHeight() * m_levelMap->TILE_SIZE - (m_app->GetGameHeight()));
}

void UnderwaterLevelState::Draw()
{
	LevelState::Draw();

	if (!m_finished)
	{
		int clamCount = GetObjectTracker()->Count<Clam>([](Clam* object) {
			return object->HasPearl();
		});

		std::string pearlText = ("Pearls x" + std::to_string(clamCount));
		DrawText(pearlText.c_str(), 8 + 1, 8, 8, BLACK);
		DrawText(pearlText.c_str(), 8 - 1, 8, 8, BLACK);
		DrawText(pearlText.c_str(), 8, 8 + 1, 8, BLACK);
		DrawText(pearlText.c_str(), 8, 8 - 1, 8, BLACK);
		DrawText(pearlText.c_str(), 8, 8, 8, WHITE);

		if (clamCount <= 0)
		{
			m_finished = true;
		}
	}
	else
	{
		DrawRectangleRec({ 0.0f,0.0f,(float)m_app->GetGameWidth(), (float)m_app->GetGameHeight() }, {0,0,0,128});
		//Center Text
		const char* text = "Complete";
		Vector2 center = MeasureTextEx(GetFontDefault(), text, 12, 1);
		center = Vector2Scale(center, 0.5f);

		Vector2 textPos = Vector2Subtract({ m_app->GetGameWidth() / 2.0f,(m_app->GetGameHeight() / 3.0f) }, center);

		DrawTextEx(GetFontDefault(), text, textPos,
			12, 1, WHITE);
	}
}
