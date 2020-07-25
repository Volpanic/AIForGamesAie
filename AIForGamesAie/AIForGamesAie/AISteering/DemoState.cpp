#include "DemoState.h"
#include "Behaviour.h"
#include "raylib.h"
#include "FollowPathBehavior.h"

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
	
	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
	{
		m_levelMap.Set((int)gridPos.x, (int)gridPos.y, 1);
	}

	if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
	{
		m_path.PathAddNode(GetScaledMousePos());
	}

	if (IsKeyDown(KEY_SPACE))
	{
		Agent* myAge = dynamic_cast<Agent*>(m_objectTracker[typeid(Agent)].front());
		myAge->SetBehaviour(new FollowPathBehavior(m_path,500));
	}

	LevelState::Update(deltaTime);
}

void DemoState::Draw()
{
	LevelState::Draw();
	DrawText("Demo",4,4,12,GRAY);

	m_path.DrawPath();
	m_levelMap.Draw();
}