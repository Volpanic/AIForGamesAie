#include "MenuState.h"
#include "raylib.h"
#include "Application.h"
#include "GameStateManager.h"
#include "Numbers.h"
#include "ObjectFactory.h"
#include "Application.h"
#include "GameStateManager.h"
#include "UnderwaterLevelState.h"
#include "FadeTransition.h"
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
	m_selectionAlpha = Lerp(m_selectionAlpha, 64.0f, 2.0f * deltaTime);
	m_timer += deltaTime;
}

void MenuState::Draw()
{
	Color textCol = { 255, 255, 255, 255};
	Color invert = { 255 - textCol.r,255 - textCol.g, 255 - textCol.b,255 };
	//Clear background with inverted textcol
	ClearBackground(invert);

	if (IsKeyPressed(KEY_DOWN))
	{
		m_menuPos++;
		m_selectionAlpha = 0;
		m_timer = 0;
	}

	if (IsKeyPressed(KEY_UP))
	{
		m_menuPos--;
		m_selectionAlpha = 0;
		m_timer = 0;
	}

	m_menuPos = Numbers::WrapValue<int>(m_menuPos,0,m_menuOptsSize);

	if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER))
	{
		switch (m_menuPos)
		{
			case 0: // New Game
			{
				ObjectFactory* of = new ObjectFactory();
				m_app->GetGameStateManager()->SetState("Level", new UnderwaterLevelState(m_app, "Rooms\\FinalLevelProgress2.xml", of));
				m_app->GetGameStateManager()->PushState("Level", new FadeTransition(m_app, "Level", 1.0f));
				delete of;
				break;
			}

			case 1: // Demo
			{

				break;
			}

			case 2: // Editor
			{

				m_app->GetGameStateManager()->PushState("Editor",new FadeTransition(m_app,"Editor",1.0f));
				break;
			}
		}
	}

	if (m_timer >= 1.0f)
	{
		m_selectionAlpha = 0.0f;
		m_timer = 0;
	}

	//Draw Menu
	for (int i = 0; i < m_menuOptsSize; i++)
	{
		//Center Text
		Vector2 center = MeasureTextEx(GetFontDefault(),m_menuOpts[i].c_str(),12,1);
		center = Vector2Scale(center, 0.5f);
		
		Vector2 textPos = Vector2Subtract({ m_app->GetGameWidth() / 2.0f,(m_app->GetGameHeight() / 2.0f) + 12.0f * i},center);

		//Draw selection cursor
		if (i == m_menuPos)
		{
			DrawRectangleRec({ 0,textPos.y,(float)m_app->GetGameWidth(),12.0f }, { textCol.r,textCol.g,textCol.b,(unsigned char)m_selectionAlpha});
		}

		DrawTextEx(GetFontDefault(), m_menuOpts[i].c_str(), textPos,
			12,1, textCol);
	}
}