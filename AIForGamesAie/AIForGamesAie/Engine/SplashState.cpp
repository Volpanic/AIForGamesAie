#include "SplashState.h"
#include "raylib.h"
#include "Application.h"
#include "GameStateManager.h"

SplashState::SplashState(Application* app) : IGameState::IGameState(app)
{

}

SplashState::~SplashState()
{

}

void SplashState::Load()
{

}

void SplashState::Unload()
{

}

void SplashState::Update(float deltaTime)
{
	m_timer += deltaTime;

	if (m_timer > 1.0f)
	{
		m_app->GetGameStateManager()->PopState();
		m_app->GetGameStateManager()->PushState("Level");
	};
}

void SplashState::Draw()
{
	DrawText("SplashScreen", 32, 32, 12, LIGHTGRAY);
}