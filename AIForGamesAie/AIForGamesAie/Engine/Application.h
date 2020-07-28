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

	//Getters
	const int& GetGameWidth() { return m_gameWidth; };
	const int& GetGameHeight() { return m_gameHeight; };
	Vector2 GetScaledMousePos() { return GetMousePosition(); };
	GameStateManager* GetGameStateManager() { return m_gameStateManager; }

protected:
	int m_gameWidth;
	int m_gameHeight;
	int m_gameZoom;

	RenderTexture2D m_gameSurface;
	GameStateManager* m_gameStateManager = nullptr;

private:
	

};