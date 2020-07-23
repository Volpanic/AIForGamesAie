#pragma once
#include "raylib.h"

class GameStateManager;

class Application
{
public:
	Application(int gameWidth, int gameHeight, int gameZoom);
	~Application();

	void Run();
	void Update(float deltaTime);
	void Draw();

	GameStateManager* GetGameStateManager() { return m_gameStateManager; }

protected:

private:
	int m_gameWidth;
	int m_gameHeight;
	int m_gameZoom;

	RenderTexture2D m_gameSurface;
	GameStateManager* m_gameStateManager = nullptr;

};