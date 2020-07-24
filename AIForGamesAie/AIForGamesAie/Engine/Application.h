#pragma once
#include "raymath.h"
#include "raylib.h"

class GameStateManager;

class Application
{
public:
	Application(int gameWidth, int gameHeight, int gameZoom);
	virtual ~Application();

	virtual void Create();
	void Run();
	void Update(float deltaTime);
	void Draw();

	Vector2 GetScaledMousePos() { return Vector2Divide(GetMousePosition(), { (float)m_gameZoom,(float)m_gameZoom }); };

	GameStateManager* GetGameStateManager() { return m_gameStateManager; }

protected:
	int m_gameWidth;
	int m_gameHeight;
	int m_gameZoom;

	RenderTexture2D m_gameSurface;
	GameStateManager* m_gameStateManager = nullptr;

private:
	

};