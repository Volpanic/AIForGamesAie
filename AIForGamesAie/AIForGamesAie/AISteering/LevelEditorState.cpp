#include "LevelEditorState.h"
#include "Behaviour.h"
#include "raylib.h"
#include "FollowPathBehavior.h"
#include "Graph2D.h"
#include "Graph2DEditor.h"

#include "raygui.h"
#include "ricons.h"

LevelEditorState::LevelEditorState(Application* app) : LevelState::LevelState(app)
{
	m_graphEditor = new Graph2DEditor();
	m_graph = new Graph2D();

	m_graphEditor->SetGrapth(m_graph);
}

LevelEditorState::~LevelEditorState()
{
	delete m_graphEditor;
	delete m_graph;
}

Vector2 LevelEditorState::EditorMousePos()
{
	Vector2 pos = m_app->GetScaledMousePos();
	pos.x = floor(pos.x);
	pos.y = floor(pos.y);

	if (m_snappedToGrid)
	{
		Vector2 gridPos = m_levelMap.ToGridPos(pos);
		return Vector2Scale(gridPos,m_levelMap.TILE_SIZE);
	}
	else
	{
		return pos;
	}
}

void LevelEditorState::Update(float deltaTime)
{
	if (IsKeyDown(KEY_C))
	{
		return;
	}

	m_snappedToGrid = IsKeyDown(KEY_LEFT_CONTROL);

	switch (m_editorState)
	{
		case EditorStates::Tiles:
		{
			Vector2 gridPos = m_levelMap.ToGridPos(EditorMousePos());

			//Create
			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
			{
				m_levelMap.Set((int)gridPos.x, (int)gridPos.y, 1);
			}

			//Delete
			if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
			{
				m_levelMap.Set((int)gridPos.x, (int)gridPos.y, 0);
			}

			break;
		}

		case EditorStates::Nodes:
		{
			m_graphEditor->Update(EditorMousePos(),deltaTime);
			break;
		}
	}

	//LevelState::Update(deltaTime);
}

void LevelEditorState::Draw()
{
	if (m_drawGrid)
	{
		for (int xx = 0; xx < m_levelMap.GetWidth(); xx++)
		{
			DrawLine(xx * m_levelMap.TILE_SIZE, 0, xx * m_levelMap.TILE_SIZE, m_levelMap.GetHeight() * m_levelMap.TILE_SIZE, RAYWHITE);
		}

		for (int yy = 0; yy < m_levelMap.GetHeight(); yy++)
		{
			DrawLine(0, yy * m_levelMap.TILE_SIZE, m_levelMap.GetWidth() * m_levelMap.TILE_SIZE, yy * m_levelMap.TILE_SIZE, RAYWHITE);
		}
	}

	LevelState::Draw();
	m_levelMap.Draw();

	if (IsKeyDown(KEY_C))
	{
		Rectangle buttonRect = { 8,8,48,16 };

		if (GuiButton(buttonRect, GuiIconText(RICON_BOX_GRID,"")))
		{
			m_editorState = EditorStates::Tiles;
		}
		buttonRect.y += 20;

		if (GuiButton(buttonRect, GuiIconText(RICON_CURSOR_HAND, "")))
		{
			m_editorState = EditorStates::Nodes;
		}
		buttonRect.y += 20;

		if (GuiButton(buttonRect, GuiIconText(RICON_PLAYER_PLAY, "")))
		{
			m_editorState = EditorStates::Entities;
		}
		buttonRect.y += 20;

		Rectangle toggleRed = { 8,m_app->GetGameHeight() - 20,16,16 };

		if (GuiButton(toggleRed, GuiIconText(RICON_GRID,"")))
		{
			m_drawGrid = !m_drawGrid;
		}
	}
	else
	{
		switch (m_editorState)
		{
			case EditorStates::Tiles:
			{
				Vector2 gridPos = m_levelMap.ToGridPos(m_app->GetScaledMousePos());

				DrawRectangleLinesEx({gridPos.x * m_levelMap.TILE_SIZE,gridPos.y * m_levelMap.TILE_SIZE,(float)m_levelMap.TILE_SIZE ,(float)m_levelMap.TILE_SIZE },1,LIGHTGRAY);

				break;
			}

			case EditorStates::Nodes:
			{
				m_graphEditor->Draw();
				break;
			}
		}
	}
}