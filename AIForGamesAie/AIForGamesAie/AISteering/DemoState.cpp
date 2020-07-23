#include "DemoState.h"
#include "raylib.h"

DemoState::DemoState(Application* application) : m_app(application)
{
	
}

DemoState::~DemoState()
{

}

void DemoState::Update(float deltaTime)
{
	agent.Update(deltaTime);
}

void DemoState::Draw()
{
	DrawText("Demo",4,4,12,GRAY);
	agent.Draw();
}