#include "Game.h"
#include "Application.h"
#include "GameStateManager.h"
#include "DemoState.h"
#include "FadeTransition.h"
#include "LevelEditorState.h"
#include "cimgui_impl_raylib.h"
#include "ObjectFactory.h"
#include "Numbers.h"
#include "rlgl.h"

Game::Game(int gameWidth, int gameHeight, int gameZoom) : Application::Application(gameWidth,gameHeight,gameZoom)
{
	
}

Game::~Game()
{

}

void Game::Create()
{
	//Change to switch from editor to player ya hear?
	if (m_Editor)
	{
		//ImGui
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplRaylib_Init();

		//Build Texture atlas
		int width;
		int height;

		ImGuiContext* ctx;
		ImGuiIO& io = ImGui::GetIO();
		ImGui::GetIO().DisplaySize = { (float)(GetGameWidth() * m_gameZoom),(float)(GetGameHeight() * m_gameZoom) };

		unsigned char* pixels;
		io.Fonts->AddFontDefault();
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, NULL);

		unsigned int texID = rlLoadTexture(pixels, width, height, PixelFormat::UNCOMPRESSED_R8G8B8A8, 1);

		io.Fonts->TexID = (ImTextureID)texID;

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::GetStyle().AntiAliasedLines = false;
		//ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowBorderSize, -1.0f);
		//ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_ChildBorderSize, -1.0f);
		//ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_PopupBorderSize, -1.0f);
		//ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FrameBorderSize, -1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FrameRounding, 2.0f);

		//ImGui

		m_gameStateManager->SetState("Editor", new LevelEditorState(this));
		m_gameStateManager->PushState("Editor", new FadeTransition(this, "Editor", 0.25f));
		SetConfigFlags(FLAG_WINDOW_RESIZABLE);

		UnloadRenderTexture(m_gameSurface);
		m_gameSurface = LoadRenderTexture(m_gameWidth*2, m_gameHeight*2);
	}
	else
	{
		ObjectFactory* of = new ObjectFactory();
		m_gameStateManager->SetState("Level", new LevelState(this,"Rooms\\dummy.xml",of));
		m_gameStateManager->PushState("Level");
		delete of;
	}
}

void Game::StartDraw()
{
	if (m_Editor)
	{
		ImGui_ImplRaylib_NewFrame(m_gameZoom);
		ImGui_ImplRaylib_ProcessEvent();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(0);
	}
}

void Game::EndDraw()
{
	if (m_Editor)
	{
		ImGui::Render();
		m_drawData = ImGui::GetDrawData();
		raylib_render_cimgui(m_drawData);
	}
}
