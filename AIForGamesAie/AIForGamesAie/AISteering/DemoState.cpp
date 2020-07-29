#include "DemoState.h"
#include "Behaviour.h"
#include "raylib.h"
#include "FollowPathBehavior.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"

DemoState::DemoState(Application* app) : LevelState::LevelState(app)
{
	Add<Agent>(new Agent(this));
}

DemoState::~DemoState()
{

}

void DemoState::Update(float deltaTime)
{
	Vector2 gridPos = m_levelMap->ToGridPos(m_app->GetScaledMousePos());
	
	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
	{
		m_levelMap->Set((int)gridPos.x, (int)gridPos.y, 1);
	}

	if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
	{
		m_path.PathAddNode(GetScaledMousePos());

	}

	if (IsKeyPressed(KEY_Z))
	{
		Agent* agent = Add<Agent>(new Agent(this));
		agent->SetPosition(GetScaledMousePos().x, GetScaledMousePos().y);
	}

	if (IsKeyDown(KEY_SPACE))
	{
		Agent* myAge = GetAll<Agent>().front();
		myAge->SetBehaviour(new FollowPathBehavior(m_path,500));
	}

	if (IsKeyPressed(KEY_BACKSPACE))
	{
		Agent* myAge = GetAll<Agent>().front();
		Remove<Agent>(myAge);
	}

	LevelState::Update(deltaTime);
}

void DemoState::Draw()
{
	guiFont.baseSize = 12;

	LevelState::Draw();

	m_path.DrawPath();
	m_levelMap->Draw();

	DrawCircleV(GetScaledMousePos(), 4, RED);

}