#include "Game.h"
#include "Application.h"
#include "GameStateManager.h"
#include "DemoState.h"
#include "FadeTransition.h"
#include "LevelEditorState.h"

Game::~Game()
{

}


void Game::Create()
{
	m_gameStateManager->SetState("Editor",new LevelEditorState(this));
	m_gameStateManager->PushState("Editor",new FadeTransition(this,"Editor",0.25f));
}