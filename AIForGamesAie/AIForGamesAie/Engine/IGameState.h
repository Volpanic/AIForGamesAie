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

	Application* GetGameApp() { return m_app;};
	GameStateManager* GetGameStateManager() { return m_app->GetGameStateManager(); };
	ResourceManager* GetResources() { return m_app->GetResources(); };

protected:
	Application* m_app;
private:
};