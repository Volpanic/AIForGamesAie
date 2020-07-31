#include "Game.h"
#include "Application.h"
#include "GameStateManager.h"
#include "DemoState.h"
#include "FadeTransition.h"
#include "LevelEditorState.h"
#include "cimgui_impl_raylib.h"
#include "Numbers.h"

Game::~Game()
{

}

void Game::Create()
{
	m_gameStateManager->SetState("Editor",new LevelEditorState(this));
	m_gameStateManager->PushState("Editor",new FadeTransition(this,"Editor",0.25f));

	//ImGui
	ImGui::CreateContext();
	ImGui::StyleColorsClassic();
	ImGui_ImplRaylib_Init();

	//Build Texture atlas
	int width = GetGameWidth() * m_gameZoom;
	int height = GetGameHeight() * m_gameZoom;

	ImGuiContext* ctx;
	auto io = ImGui::GetIO();
	ImGui::GetIO().DisplaySize = {(float)(GetGameWidth() * m_gameZoom),(float)(GetGameHeight() * m_gameZoom)};

	unsigned char* pixels = NULL;

	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, NULL);
	Image image = LoadImageEx((Color*)pixels, width, height);
	Texture2D texture = LoadTextureFromImage(image);
	io.Fonts->TexID = (void*)&texture.id;
	//ImGui
}

void Game::StartDraw()
{
	ImGui_ImplRaylib_NewFrame();
	ImGui_ImplRaylib_ProcessEvent();
	ImGui::NewFrame();
}

void Game::EndDraw()
{
	ClearBackground({0,0,0,0});

	ImGui::Render();
	m_drawData = ImGui::GetDrawData();
	raylib_render_cimgui(m_drawData);
}
