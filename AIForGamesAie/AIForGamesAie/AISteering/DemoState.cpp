#include "DemoState.h"
#include "raylib.h"

DemoState::DemoState(Application* application) : m_app(application ),LevelState::LevelState(application)
{
	m_objectTracker[typeid(Agent)].push_back(new Agent(this));
}

DemoState::~DemoState()
{
	for (auto const& objList : m_objectTracker)
	{
		for (auto obj = objList.second.begin(); obj != objList.second.end(); obj++)
		{
			delete (*obj);
		}
	}
}

void DemoState::Update(float deltaTime)
{
	Vector2 gridPos = m_levelMap.ToGridPos(m_app->GetScaledMousePos());
	
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		m_levelMap.Set((int)gridPos.x, (int)gridPos.y, 1);
	}

	LevelState::Update(deltaTime);
}

void DemoState::Draw()
{
	LevelState::Draw();
	DrawCircleV(m_targetPos, 4, LIGHTGRAY);
	DrawText("Demo",4,4,12,GRAY);

	m_levelMap.Draw();
}