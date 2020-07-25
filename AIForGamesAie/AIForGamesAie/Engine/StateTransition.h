#pragma once
#include "Application.h"

class StateTransition
{
public:
	StateTransition(Application* app, const char* stateName) { m_app = app; m_stateName = stateName;};
	virtual ~StateTransition() {};
	virtual void Update(float deltaTime) {};
	virtual void Draw() {};

	bool IsComplete() { return m_complete; };

protected:
	bool m_complete = false;
	Application* m_app;
	const char* m_stateName;

private:
	
};

