#include "Game.h"
#include "Application.h"
#include "GameStateManager.h"
#include "DemoState.h"
#include "FadeTransition.h"
#include "LevelEditorState.h"
#include "ObjectFactory.h"
#include "Numbers.h"
#include "UnderwaterLevelState.h"
#include "rlgl.h"
#include "MenuState.h"

Game::Game(int gameWidth, int gameHeight, int gameZoom) : Application::Application(gameWidth,gameHeight,gameZoom)
{
	
}

Game::~Game()
{

}

void Game::Create()
{

	m_gameStateManager->SetState("Editor", new LevelEditorState(this));
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	//UnloadRenderTexture(m_gameSurface);
	//m_gameSurface = LoadRenderTexture(m_gameWidth*2, m_gameHeight*2);

	m_gameStateManager->SetState("MainMenu", new MenuState(this));
	m_gameStateManager->PushState("MainMenu");
}

void Game::StartDraw()
{
}

void Game::EndDraw()
{

}
