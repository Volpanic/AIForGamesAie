#include "Game.h"
#include "Application.h"
#include "GameStateManager.h"
#include "FadeTransition.h"
#include "LevelEditorState.h"
#include "ObjectFactory.h"
#include "Numbers.h"
#include "rlgl.h"

#include "DemoState.h"
#include "MenuState.h"
#include "GameOverState.h"
#include "LevelEditorState.h"
#include "UnderwaterLevelState.h"

Game::Game(int gameWidth, int gameHeight, int gameZoom) : Application::Application(gameWidth,gameHeight,gameZoom)
{
	
}

Game::~Game()
{
	CloseAudioDevice();
	UnloadMusicStream(m_song);
}

void Game::Create()
{

	InitAudioDevice();
	m_song = LoadMusicStream("Music\\mus_water.wav");

	ObjectFactory* of = new ObjectFactory();
	m_gameStateManager->SetState("Level", new UnderwaterLevelState(this, "Rooms\\FinalLevel.xml", of));;
	delete of;

	m_gameStateManager->SetState("Editor", new LevelEditorState(this));
	m_gameStateManager->SetState("GameOver", new GameOverState(this));

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	m_gameStateManager->SetState("MainMenu", new MenuState(this));
	m_gameStateManager->PushState("MainMenu");

	PlayMusicStream(m_song);
}

void Game::StartDraw()
{
	UpdateMusicStream(m_song);
}

void Game::EndDraw()
{
	
}
