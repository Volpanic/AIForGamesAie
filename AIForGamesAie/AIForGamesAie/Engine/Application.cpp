#include "Application.h"
#include "raylib.h"
#include "GameStateManager.h"
#include "SplashState.h"
#include "MenuState.h"
#include "LevelState.h"

Application::Application(int gameWidth, int gameHeight, int gameZoom)
{
	m_gameWidth = gameWidth;
	m_gameHeight = gameHeight;
	m_gameZoom = gameZoom;
}

Application::~Application()
{
	UnloadRenderTexture(m_gameSurface);
	m_resourceManger->Unload();
	delete m_resourceManger;
}

void Application::Create()
{
	//Games States
	m_gameStateManager->SetState("Splash", new SplashState(this));

	m_gameStateManager->SetState("Level", new LevelState(this));
	m_gameStateManager->PushState("Splash");
}

void Application::Run()
{
	InitWindow(m_gameWidth * m_gameZoom, m_gameHeight * m_gameZoom, "Voyage ep");
	//SetTargetFPS(60);

	//Render textures
	m_gameSurface = LoadRenderTexture(m_gameWidth,m_gameHeight);
	m_gameStateManager = new GameStateManager();
	
	m_resourceManger->Init();
	Create();

	SetMouseScale(1.0f/m_gameZoom, 1.0f/m_gameZoom);

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();
		
		StartUpdate(dt);

			Update(dt);

		EndUpdate(dt);

		Draw();
	}

	delete m_gameStateManager;

	CloseWindow();
}

void Application::Update(float deltaTime)
{
	m_gameStateManager->Update(deltaTime);
}

void Application::Draw()
{
	BeginDrawing();
	StartDraw();

	BeginTextureMode(m_gameSurface);

	ClearBackground(WHITE);
	
	m_gameStateManager->Draw();

	EndTextureMode();

	//Draw Upscaled texture
	DrawTexturePro(m_gameSurface.texture, { 0.0f,0.0f,(float)m_gameWidth,(float)-m_gameHeight },
		{ 0.0f,0.0f,(float)(m_gameWidth * m_gameZoom),(float)(m_gameHeight * m_gameZoom) }, { 0,0 }, 0.0f, WHITE);

	m_gameStateManager->EndDraw();

	EndDraw();

	EndDrawing();
}