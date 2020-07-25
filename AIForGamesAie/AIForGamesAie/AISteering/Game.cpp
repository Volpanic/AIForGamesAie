#include "Game.h"
#include "Application.h"
#include "GameStateManager.h"
#include "DemoState.h"
#include "FadeTransition.h"

Game::~Game()
{

}


void Game::Create()
{
	m_gameStateManager->SetState("Demo",new DemoState(this));
	m_gameStateManager->PushState("Demo",new FadeTransition(this,"Demo",1.0f));
}