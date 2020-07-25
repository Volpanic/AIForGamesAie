#include "FadeTransition.h"
#include "GameStateManager.h"
#include "Numbers.h"
#include <iostream>

FadeTransition::FadeTransition(Application* app, const char* stateName, float fadeTime) : StateTransition::StateTransition(app,stateName)
{
	m_fadeTime = fadeTime;
}

FadeTransition::~FadeTransition()
{

}

void FadeTransition::Update(float deltaTime)
{
	if (m_fadeIn)
	{
		//Approach timer to target, fading in
		if (m_tranTimer >= m_fadeTime)
		{
			//Change state and fade out
			m_app->GetGameStateManager()->PushState(m_stateName);
			m_fadeIn = false;
		}
		m_tranTimer = Numbers::Approach(m_tranTimer, m_fadeTime, deltaTime);
		
	}
	else
	{
		//Fading out
		if (m_tranTimer <= 0)
		{
			//Complete
			m_complete = true;
		}
		m_tranTimer = Numbers::Approach(m_tranTimer, 0.0f, deltaTime);
	}
}

void FadeTransition::Draw() 
{
	Color col = BLACK;
	//Convert m_trantimer to alpha;
	col.a = ((m_tranTimer / m_fadeTime)*255);

	//Draw transparent rectangle with alpha value.
	DrawRectangle(0, 0, m_app->GetGameWidth(), m_app->GetGameHeight(), col);
}
