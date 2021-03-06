#pragma once
#include "imgui.h"
#include "rlgl.h"
#include "raylib.h"


static double g_time = 0.0;

static const char* ImGui_ImplRaylib_GetClipboardText(void* user_data)
{
	return GetClipboardText();
}

static void ImGui_ImplRaylib_SetClipboardText(void* user_data,const char* text)
{
	SetClipboardText(text);
}

//  ImGui_ImplRaylib

static void ImGui_ImplRaylib_Init()
{
	ImGuiIO& io = ImGui::GetIO();
	io.BackendPlatformName = "imgui_impl_raylib++";

	io.KeyMap[ImGuiKey_Tab] = KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = KEY_HOME;
	io.KeyMap[ImGuiKey_End] = KEY_END;
	io.KeyMap[ImGuiKey_Insert] = KEY_INSERT;
	io.KeyMap[ImGuiKey_Delete] = KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = KEY_SPACE;
	io.KeyMap[ImGuiKey_Enter] = KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = KEY_ESCAPE;
	io.KeyMap[ImGuiKey_KeyPadEnter] = KEY_ENTER;
	io.KeyMap[ImGuiKey_A] = KEY_A;
	io.KeyMap[ImGuiKey_C] = KEY_B;
	io.KeyMap[ImGuiKey_V] = KEY_V;
	io.KeyMap[ImGuiKey_X] = KEY_X;
	io.KeyMap[ImGuiKey_Y] = KEY_Y;
	io.KeyMap[ImGuiKey_Z] = KEY_Z;

	io.SetClipboardTextFn = ImGui_ImplRaylib_SetClipboardText;
	io.GetClipboardTextFn = ImGui_ImplRaylib_GetClipboardText;

	
}

static void ImGui_implRaylib_UpdateMouseCursor()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGuiMouseCursor imguiCursor = ImGui::GetMouseCursor();
	if (io.MouseDrawCursor || imguiCursor == ImGuiMouseCursor_None)
	{
		HideCursor();
	}
	else
	{
		ShowCursor();
	}
}

static void ImGui_implRaylib_UpdateMousePosAndButtons(float mouseScale)
{
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantSetMousePos)
	{
		SetMousePosition(io.MousePos.x, io.MousePos.y);
	}
	else
	{
		io.MousePos = {(float)GetMouseX()*mouseScale,(float)GetMouseY() * mouseScale };
	}

	io.MouseDown[0] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
	io.MouseDown[1] = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
	io.MouseDown[2] = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);
}

static void ImGui_ImplRaylib_NewFrame(float mouseScale)
{
	ImGuiIO& io = ImGui::GetIO();
	IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

	io.DisplaySize = {(float) GetScreenWidth(), (float)GetScreenHeight()};

	double current_time = GetTime();
	io.DeltaTime = g_time > 0.0 ? (float)( current_time - g_time) : (float)(1.0f / 60.0f);

	ImGui_implRaylib_UpdateMousePosAndButtons(mouseScale);
	ImGui_implRaylib_UpdateMouseCursor();

	if (GetMouseWheelMove() > 0)
		io.MouseWheel += 1;
	else if (GetMouseWheelMove() < 0)
		io.MouseWheel -= 1;
}

#define FOR_ALL_KEYS(X) \
X(KEY_APOSTROPHE); \
X(KEY_QUESTION); \
X(KEY_COMMA); \
X(KEY_MINUS); \
X(KEY_PERIOD); \
X(KEY_SLASH); \
X(KEY_ZERO); \
X(KEY_ONE); \
X(KEY_TWO); \
X(KEY_THREE); \
X(KEY_FOUR); \
X(KEY_FIVE); \
X(KEY_SIX); \
X(KEY_SEVEN); \
X(KEY_EIGHT); \
X(KEY_NINE); \
X(KEY_SEMICOLON); \
X(KEY_EQUAL); \
X(KEY_A); \
X(KEY_B); \
X(KEY_C); \
X(KEY_D); \
X(KEY_E); \
X(KEY_F); \
X(KEY_G); \
X(KEY_H); \
X(KEY_I); \
X(KEY_J); \
X(KEY_K); \
X(KEY_L); \
X(KEY_M); \
X(KEY_N); \
X(KEY_O); \
X(KEY_P); \
X(KEY_Q); \
X(KEY_R); \
X(KEY_S); \
X(KEY_T); \
X(KEY_U); \
X(KEY_V); \
X(KEY_W); \
X(KEY_X); \
X(KEY_Y); \
X(KEY_Z); \
X(KEY_SPACE); \
X(KEY_ESCAPE); \
X(KEY_ENTER); \
X(KEY_TAB); \
X(KEY_BACKSPACE); \
X(KEY_INSERT); \
X(KEY_DELETE); \
X(KEY_RIGHT); \
X(KEY_LEFT); \
X(KEY_DOWN); \
X(KEY_UP); \
X(KEY_PAGE_UP); \
X(KEY_PAGE_DOWN); \
X(KEY_HOME); \
X(KEY_END); \
X(KEY_CAPS_LOCK); \
X(KEY_SCROLL_LOCK); \
X(KEY_NUM_LOCK); \
X(KEY_PRINT_SCREEN); \
X(KEY_PAUSE); \
X(KEY_F1); \
X(KEY_F2); \
X(KEY_F3); \
X(KEY_F4); \
X(KEY_F5); \
X(KEY_F6); \
X(KEY_F7); \
X(KEY_F8); \
X(KEY_F9); \
X(KEY_F10); \
X(KEY_F11); \
X(KEY_F12); \
X(KEY_LEFT_SHIFT); \
X(KEY_LEFT_CONTROL); \
X(KEY_LEFT_ALT); \
X(KEY_LEFT_SUPER); \
X(KEY_RIGHT_SHIFT); \
X(KEY_RIGHT_CONTROL); \
X(KEY_RIGHT_ALT); \
X(KEY_RIGHT_SUPER); \
X(KEY_KB_MENU); \
X(KEY_LEFT_BRACKET); \
X(KEY_BACKSLASH); \
X(KEY_RIGHT_BRACKET); \
X(KEY_GRAVE); \
X(KEY_KP_0); \
X(KEY_KP_1); \
X(KEY_KP_2); \
X(KEY_KP_3); \
X(KEY_KP_4); \
X(KEY_KP_5); \
X(KEY_KP_6); \
X(KEY_KP_7); \
X(KEY_KP_8); \
X(KEY_KP_9); \
X(KEY_KP_DECIMAL); \
X(KEY_KP_DIVIDE); \
X(KEY_KP_MULTIPLY); \
X(KEY_KP_SUBTRACT); \
X(KEY_KP_ADD); \
X(KEY_KP_ENTER); \
X(KEY_KP_EQUAL);

#define SET_KEY_DOWN(KEY) io.KeysDown[KEY] = IsKeyDown(KEY)

static bool ImGui_ImplRaylib_ProcessEvent()
{
	ImGuiIO& io = ImGui::GetIO();

    for (auto const& key : io.KeysDown)
    {
        SET_KEY_DOWN(key);
    }

    int keyPressed = GetKeyPressed();
    if (keyPressed > 0)
    {
        io.AddInputCharacter(keyPressed);
    }

    return true;
}

static void raylib_render_cimgui(ImDrawData* draw_data)
{
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);

    if (fb_width <= 0 || fb_height <= 0)
    {
        return;
    }

    ImVec2 clip_off = draw_data->DisplayPos;
    ImVec2 clip_scale = draw_data->FramebufferScale;

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

            //Key and eye here
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
            {
                //Most code from vulken ||m openGl3 example
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                {
					BeginScissorMode((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));
					
                }
            }
			EndScissorMode();
			//idx_buffer += pcmd->ElemCount;
        }
    }
}


