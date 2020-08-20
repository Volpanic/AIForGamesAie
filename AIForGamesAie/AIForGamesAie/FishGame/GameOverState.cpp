#include "GameOverState.h"
#include "GameStateManager.h"
#include "FadeTransition.h"
#include "MenuState.h"

GameOverState::GameOverState(Application* app) : IGameState(app)
{

}

GameOverState::~GameOverState()
{
}

void GameOverState::Load()
{
}

void GameOverState::Unload()
{
}

void GameOverState::Update(float deltaTime)
{
	if ((!m_hasInputed) && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Z)))
	{
		m_app->GetGameStateManager()->SetState("MainMenu", new MenuState(m_app));
		m_app->GetGameStateManager()->PushState("MainMenu", new FadeTransition(m_app , "MainMenu",1.0f));
		m_hasInputed = true;
	}
}

void GameOverState::Draw()
{
	ClearBackground(BLACK);

	//Center Text
	const char* text = "Game Over";
	Vector2 center = MeasureTextEx(GetFontDefault(), text, 12, 1);
	center = Vector2Scale(center, 0.5f);

	Vector2 textPos = Vector2Subtract({ m_app->GetGameWidth() / 2.0f,(m_app->GetGameHeight() / 3.0f)}, center);

	DrawTextEx(GetFontDefault(), text, textPos,
		12, 1, WHITE);
}

void GameOverState::EndDraw()
{
}
