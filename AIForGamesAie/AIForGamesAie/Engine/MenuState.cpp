#include "MenuState.h"
#include "raylib.h"
#include "Application.h"
#include "GameStateManager.h"
#include <iostream>

MenuState::MenuState(Application* app) : IGameState::IGameState(app)
{

}

MenuState::~MenuState()
{

}

void MenuState::Load()
{
	
}

void MenuState::Unload()
{

}

void MenuState::Update(float deltaTime)
{

}

void MenuState::Draw()
{
	DrawText("Menu", 32, 32, 12, LIGHTGRAY);
}