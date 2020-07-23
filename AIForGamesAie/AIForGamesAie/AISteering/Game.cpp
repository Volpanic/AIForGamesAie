#include "Game.h"
#include "Application.h"
#include "GameStateManager.h"
#include "DemoState.h"

Game::~Game()
{

}


void Game::Create()
{
	m_gameStateManager->SetState("Demo",new DemoState(this));
	m_gameStateManager->PushState("Demo");
}