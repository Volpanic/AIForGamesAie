#pragma once
#include "Application.h"
#include "ResourceManager.h"

class IGameState
{
public:
	IGameState(Application* app) { m_app = app; };
	virtual ~IGameState() { };

	virtual void Load() { };
	virtual void Unload() { };

	virtual void Update(float deltaTime) { };
	virtual void Draw() { };
	virtual void EndDraw() {};

	ResourceManager* GetResources() { return m_app->GetResources(); };

protected:
	Application* m_app;
private:
};