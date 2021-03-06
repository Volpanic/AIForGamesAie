#include "DemoState.h"
#include "Behaviour.h"
#include "raylib.h"
#include "FollowPathBehavior.h"

DemoState::DemoState(Application* app) : LevelState::LevelState(app)
{
	//Add<Agent>(new Agent(this));
}

DemoState::DemoState(Application* app, std::string loadFileName, GameObjectFactory* factory) : LevelState::LevelState(app,loadFileName,factory)
{
}

DemoState::~DemoState()
{

}

void DemoState::Update(float deltaTime)
{
	Vector2 gridPos = m_levelMap->ToGridPos(m_app->GetScaledMousePos());

	if (IsKeyPressed(KEY_Z))
	{
		//Agent* agent = Add<Agent>(new Agent(this));
		//agent->SetPosition(GetScaledMousePos().x, GetScaledMousePos().y);
	}

	if (IsKeyDown(KEY_SPACE))
	{
		//Agent* myAge = GetAll<Agent>().front();
		//myAge->SetBehaviour(new FollowPathBehavior(&m_path,500));
	}

	if (IsKeyPressed(KEY_BACKSPACE))
	{
		//Agent* myAge = GetAll<Agent>().front();
		//Remove<Agent>(myAge);
	}

	LevelState::Update(deltaTime);
}

void DemoState::Draw()
{

	LevelState::Draw();

	m_path.DrawPath();
	m_levelMap->Draw(this);
}