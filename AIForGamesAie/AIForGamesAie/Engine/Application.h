#pragma once
#include "raymath.h"
#include "raylib.h"
#include "ResourceManager.h"

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
	ResourceManager* GetResources() { return m_resourceManger; };
	Vector2 GetScaledMousePos() { return GetMousePosition(); };
	GameStateManager* GetGameStateManager() { return m_gameStateManager; }

protected:
	int m_gameWidth = 320;
	int m_gameHeight = 180;
	int m_gameZoom = 4;

	RenderTexture2D m_gameSurface = {};
	GameStateManager* m_gameStateManager = nullptr;
	ResourceManager* m_resourceManger = new ResourceManager();

private:
	

};