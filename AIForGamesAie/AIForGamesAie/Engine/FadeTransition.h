#pragma once
#include "raylib.h"
#include "StateTransition.h"

class FadeTransition : public StateTransition
{
public:
	FadeTransition(Application* app, const char* stateName,float fadeTime);
	virtual ~FadeTransition();
	virtual void Update(float deltaTime);
	virtual void Draw();
private:
	float m_fadeTime = 1.0f;
	float m_tranTimer = 0;
	bool m_fadeIn = true;
};

