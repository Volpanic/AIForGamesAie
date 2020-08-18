#include "LevelEditorState.h"
#include "raylib.h"
#include "FollowPathBehavior.h"
#include "Graph2DEditor.h"
#include "Numbers.h"
#include "FollowPathBehavior.h"
#include "ObjectFactory.h"
#include "GameStateManager.h"
#include "UnderwaterLevelState.h"

#include "ObjectTracker.h"

#include "tinyxml2.h"
//#include "imgui.h"
#include "cimgui_impl_raylib.h"

//Un-godly huge file
LevelEditorState::LevelEditorState(Application* app) : LevelState::LevelState(app)
{
	//Change to switch from editor to player ya hear?

//ImGui
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplRaylib_Init();

	//Build Texture atlas
	int width;
	int height;

	ImGuiContext* ctx;
	ImGuiIO& io = ImGui::GetIO();
	ImGui::GetIO().DisplaySize = { (float)(m_app->GetGameWidth() * m_app->GetGameZoom()),(float)(m_app->GetGameHeight() * m_app->GetGameZoom()) };

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

	m_objectTracker = new ObjectTracker();
	m_camera.zoom = 1;
	m_camera.target =  Vector2Subtract(m_camera.offset, {m_app->GetGameWidth()/2.0f,m_app->GetGameHeight()/2.0f});
	m_levelMap = new LevelMap(20, 12, app);

	m_graphEditor = new Graph2DEditor(this);
	m_graph = new Graph2D();

	m_graphEditor->SetGrapth(m_graph);
	m_camera.zoom = 1;

	m_mapClearColour[0] = 1;
	m_mapClearColour[1] = 1;
	m_mapClearColour[2] = 1;

	m_tilesetsToSelect = m_app->GetResources()->GetAllTilesetNames();

	UpdateRoomFilePaths();
	m_objectFactory = new ObjectFactory();
	m_objectFactory->GetAllGameObjectTypes(m_gameObjectIDsList);

	m_levelMapWidth = m_levelMap->GetWidth();
	m_levelMapHeight = m_levelMap->GetHeight();
}

LevelEditorState::~LevelEditorState()
{
	delete m_graphEditor;
	//if(m_graph != NULL) delete m_graph; // Some other things may take the graph ya hear?
	delete m_mapClearColour;
	delete m_objectFactory;

	if (m_entityToPlace != nullptr) delete m_entityToPlace;

	//delete m_drawData;
}

Vector2 LevelEditorState::GetWorldMousePos()
{
	Vector2 pos = Vector2Scale(m_gameWindowMousePos,1.0f/m_gameViewZoom);
	pos.x += m_camera.target.x;
	pos.y += m_camera.target.y;
	pos.x = floor(pos.x);
	pos.y = floor(pos.y);

	if (m_snappedToGrid)
	{
		return Numbers::FloorMultiple(pos,8);
	}
	else
	{
		return pos;
	}
}

void LevelEditorState::FloodFillTiles(int x, int y,  int value, int targetValue)
{
	m_levelMap->FloodFillTiles(x,y,m_selectedTileLayer,value,targetValue,m_autoTile);
}

void LevelEditorState::Update(float deltaTime)
{
	//LevelState::Update(deltaTime);

	if (!m_mouseInGameWindow)
	{
		return;
	}

	//View Panning
	if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
	{
		m_panPosition.x = GetMousePosition().x + m_camera.target.x;
		m_panPosition.y = GetMousePosition().y + m_camera.target.y;
	}

	if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON))
	{
		m_camera.target.x = floorf(m_panPosition.x - GetMousePosition().x);
		m_camera.target.y = floorf(m_panPosition.y - GetMousePosition().y);
	}

	m_snappedToGrid = IsKeyDown(KEY_LEFT_CONTROL);

	switch (m_editorState)
	{
		case EditorStates::Tiles:
		{
			Vector2 gridPos = m_levelMap->ToGridPos(GetWorldMousePos());

			switch (m_tileState)
			{
				case TilePlacementState::Pencil:
				{
					//Create
					if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
					{
						if(m_autoTile)
						{
							m_levelMap->SetAutoTile(m_selectedTileLayer, (int)gridPos.x, (int)gridPos.y, m_selectedTile);
						}
						else
						{
							m_levelMap->Set(m_selectedTileLayer, (int)gridPos.x, (int)gridPos.y, m_selectedTile);
						}
					}

					//Delete
					if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
					{
						m_levelMap->Set(m_selectedTileLayer,(int)gridPos.x, (int)gridPos.y, 0);
					}

					break;
				}

				case TilePlacementState::Rectangle:
				{
					//Set mouse if rectangle, Abysmla if else chain i know
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
					{
						//Sets tile as regular
						m_placeTileValue = MouseButton::MOUSE_LEFT_BUTTON;
						m_tileRectTopleft = gridPos;
					}
					else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
					{
						//Sets tile as empty
						m_placeTileValue = MouseButton::MOUSE_RIGHT_BUTTON;
						m_tileRectTopleft = gridPos;
					}
					else if (IsMouseButtonDown(m_placeTileValue))
					{
						m_tileRectBottemRight = gridPos;
					}
					else if (IsMouseButtonReleased(m_placeTileValue))
					{
						int setValue = m_placeTileValue;

						float x1 = fminf(m_tileRectBottemRight.x, m_tileRectTopleft.x);
						float x2 = fmaxf(m_tileRectBottemRight.x, m_tileRectTopleft.x) + 1;
						float y1 = fminf(m_tileRectBottemRight.y, m_tileRectTopleft.y);
						float y2 = fmaxf(m_tileRectBottemRight.y, m_tileRectTopleft.y) + 1;

						for (int xx = (int)x1; xx < (int)x2; xx++)
						{
							for (int yy = (int)y1; yy < (int)y2; yy++)
							{
								if (m_autoTile)
								{
									m_levelMap->SetAutoTile(m_selectedTileLayer, xx, yy, m_selectedTile);
								}
								else
								{
									m_levelMap->Set(m_selectedTileLayer, xx, yy, (m_placeTileValue == MOUSE_RIGHT_BUTTON) ? 0 : m_selectedTile);
								}
							}
						}
					}
					else
					{
						m_tileRectTopleft = gridPos;
						m_tileRectBottemRight = gridPos;
					}

					break;
				}

				case TilePlacementState::Fill:
				{
					//Set mouse if rectangle
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
					{
						if (m_levelMap->WithinGrid((int)gridPos.x, (int)gridPos.y))
						{
							int floodTarget = m_levelMap->Get(m_selectedTileLayer,(int)gridPos.x, (int)gridPos.y);
							FloodFillTiles((int)gridPos.x, (int)gridPos.y,m_selectedTile,floodTarget);
						}
					}

					break;
				}
			}

			break;
		}

		case EditorStates::Nodes:
		{
			m_graphEditor->Update(GetWorldMousePos(),deltaTime);
			break;
		}

		case EditorStates::Entities:
		{
			
			break;
		}
	}
}

void LevelEditorState::Draw()
{
	
	//StartDraw
	ImGui_ImplRaylib_NewFrame(m_app->GetGameZoom());
	ImGui_ImplRaylib_ProcessEvent();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(0);

	LevelState::Draw();

	BeginMode2D(m_camera);


	//Draw nodes in non node editor modes.
	if (m_drawNodes)
	{
		m_graphEditor->DrawOnlyNodes();
	}

	//Draw grid cells the size of 1 Tile.
	if (m_drawGrid)
	{
		for (int xx = 0; xx < m_levelMap->GetWidth(); xx++)
		{
			DrawLine(xx * m_levelMap->TILE_SIZE, 0, xx * m_levelMap->TILE_SIZE, m_levelMap->GetHeight() * m_levelMap->TILE_SIZE, RAYWHITE);
		}

		for (int yy = 0; yy < m_levelMap->GetHeight(); yy++)
		{
			DrawLine(0, yy * m_levelMap->TILE_SIZE, m_levelMap->GetWidth() * m_levelMap->TILE_SIZE, yy * m_levelMap->TILE_SIZE, RAYWHITE);
		}
	}

	//Draw grid cells the size of 1 screen.
	if (m_drawScreenGrid)
	{
		int xLevelWidths = ceil(((float)m_levelMap->GetWidth() * (float)m_levelMap->TILE_SIZE) / (float)m_app->GetGameWidth());
		int yLevelHeights = ceil(((float)m_levelMap->GetHeight() * (float)m_levelMap->TILE_SIZE) / (float)m_app->GetGameHeight());

		for (int xx = 0; xx < xLevelWidths; xx++)
		{
			for (int yy = 0; yy < yLevelHeights; yy++)
			{
				DrawRectangleLinesEx({ (float)m_app->GetGameWidth() * xx, (float)m_app->GetGameHeight() * yy, (float)m_app->GetGameWidth(), (float)m_app->GetGameHeight() },2, BLACK);
			}
		}
	}

	//Draws the level map (Tiles)
	m_levelMap->Draw(this);

	//Main Tab
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				m_saveMenuOpen = true;
			}
			
			if (ImGui::MenuItem("Load"))
			{
				m_loadMenuOpen = true;
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Run File"))
			{
				m_runMenuOpen = true;
			}

			if (ImGui::MenuItem("Run This"))
			{
				SaveMap("temp");

				m_runMenuOpen = false;

				m_gameRunning = true;
				UnderwaterLevelState* newLevel = new UnderwaterLevelState(m_app, "Rooms\\temp.xml", m_objectFactory);
				m_app->GetGameStateManager()->SetState("Level", newLevel);
				m_app->GetGameStateManager()->PushState("Level");
				
				//Deletes the temp file.
				remove("Rooms\\temp.xml");
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::Text("Toggels");
			ImGui::Checkbox("Draw Grid", &m_drawGrid);
			ImGui::Checkbox("Draw Screen Grid", &m_drawScreenGrid);
			ImGui::Checkbox("Draw Nodes", &m_drawNodes);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	//Opens the save and load modals
	if (m_saveMenuOpen)
	{
		ImGui::OpenPopup("SaveMenu");
		m_saveMenuOpen = false;
	}

	if (m_loadMenuOpen || m_runMenuOpen)
	{
		ImGui::OpenPopup("LoadMenu");
	}

	//Save Menu
	//center this window when appearing
	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_::ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("SaveMenu",NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		//set focus on text box
		if (ImGui::IsWindowAppearing())
		{
			ImGui::SetKeyboardFocusHere(0);
		}
		ImGui::InputText("File Name: Rooms/", m_saveFileName, IM_ARRAYSIZE(m_saveFileName));

		if (ImGui::Button("Save") || ImGui::IsKeyPressed(ImGuiKey_Enter))
		{
			SetGraph(m_graphEditor->GetGraph());
			SaveMap(m_saveFileName);
			UpdateRoomFilePaths();
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Close") || ImGui::IsKeyPressed(ImGuiKey_Escape))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	//center this window when appearing
	ImGui::SetNextWindowPos(center, ImGuiCond_::ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("LoadMenu", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::ListBoxHeader("Rooms");
		{
			for (auto item : m_RoomFilePaths)
			{
				ImGui::Selectable(item.filename().generic_string().c_str());
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
				{
					if (m_loadMenuOpen)
					{
						m_loadMenuOpen = false;
						LoadMap(item.generic_string(), m_objectFactory);
						m_graphEditor->SetGrapth(m_graph);

						//delete m_graphEditor;
						//m_graphEditor = new Graph2DEditor(this);
						//m_graphEditor->SetGrapth(m_graph);

						ImGui::CloseCurrentPopup();
					}
					else // Run Menu
					{
						m_runMenuOpen = false;

						m_gameRunning = true;
						UnderwaterLevelState* newLevel = new UnderwaterLevelState(m_app, item.generic_string(), m_objectFactory);
						m_app->GetGameStateManager()->SetState("Level",newLevel);
						m_app->GetGameStateManager()->PushState("Level");
					}
					break;
				}
			}

			ImGui::ListBoxFooter();
		}

		ImGui::SameLine();
		if (ImGui::Button("Close"))
		{
			m_loadMenuOpen = false;
			m_runMenuOpen = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}


	//Editor state drawing, cursors and etc
	switch (m_editorState)
	{
		case EditorStates::Tiles:
		{
			Vector2 gridPos = m_levelMap->ToGridPos(GetWorldMousePos());

			switch (m_tileState)
			{
				case TilePlacementState::Pencil:
				{
					DrawRectangleLinesEx({ gridPos.x * m_levelMap->TILE_SIZE,gridPos.y * m_levelMap->TILE_SIZE,(float)m_levelMap->TILE_SIZE ,(float)m_levelMap->TILE_SIZE }, 1, LIGHTGRAY);
					break;
				}

				case TilePlacementState::Rectangle:
				{
					float x1 = fminf(m_tileRectBottemRight.x, m_tileRectTopleft.x);
					float x2 = fmaxf(m_tileRectBottemRight.x, m_tileRectTopleft.x) + 1;
					float y1 = fminf(m_tileRectBottemRight.y, m_tileRectTopleft.y);
					float y2 = fmaxf(m_tileRectBottemRight.y, m_tileRectTopleft.y) + 1;

					Vector2 size = {x2 - x1, y2 - y1};
					size = Vector2Scale(size, m_levelMap->TILE_SIZE);
					DrawRectangleLinesEx({ x1 * m_levelMap->TILE_SIZE,y1 * m_levelMap->TILE_SIZE,size.x,size.y }, 1, LIGHTGRAY);
					break;
				}

				case TilePlacementState::Fill:
				{
					DrawRectangleLinesEx({ gridPos.x * m_levelMap->TILE_SIZE,gridPos.y * m_levelMap->TILE_SIZE,(float)m_levelMap->TILE_SIZE ,(float)m_levelMap->TILE_SIZE }, 1, LIGHTGRAY);
					break;
				}
			}
			break;
		}

		case EditorStates::Nodes:
		{
			m_graphEditor->Draw();
			break;
		}

		case EditorStates::Entities:
		{
			if (m_entityToPlace != nullptr)
			{
				m_entityToPlace->DrawInEditor(GetWorldMousePos());

				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
				{
					m_objectTracker->Add(m_entityToPlace->GetCategory(),m_entityToPlace);
					m_entityToPlace = nullptr;
				}
			}
			break;
		}
	}

	//m_objectTracker->ForEachObject([&](GameObject* object) {
	//	DrawCircleV(object->GetPosition(),4,RED);
	//	});

	EndMode2D();
}

void LevelEditorState::EndDraw()
{
	DrawRectangle(0,0,GetScreenWidth(),GetScreenHeight(),BLACK);
	//ImGui::ShowDemoWindow();
	//Controls Window
	ImGui::Begin("Controls (2)");
	{
		ImGui::BeginTabBar("Editors");
		{
			if(ImGui::BeginTabItem("Map Details"))
			{
				//Resize the map grid cells
				bool gridResize = false;
				if (ImGui::InputInt("Map Width", &m_levelMapWidth))
				{
					gridResize = true;
				}

				if (ImGui::InputInt("Map Height", &m_levelMapHeight))
				{
					gridResize = true;
				}

				if (gridResize)
				{
					m_levelMap->Resize(m_levelMapWidth,m_levelMapHeight);
				}

				ImGui::ColorEdit3("Map Clear", m_mapClearColour);

				//Saving and loading
				if (ImGui::Button("Save Map"))
				{
					m_saveMenuOpen = true;
				}

				if (ImGui::Button("Load Map"))
				{
					m_loadMenuOpen = true;
				}

				if (m_gameRunning)
				{
					if (ImGui::Button("Stop Run"))
					{
						m_app->GetGameStateManager()->PopState();
						m_gameRunning = false;
					}
					ImGui::Text(("FPS: " + std::to_string(GetFPS())).c_str());
				}

				ImGui::EndTabItem();
			}

			if(ImGui::BeginTabItem("Tiles"))
			{
				m_editorState = EditorStates::Tiles;

				//Draw tileset
				if (m_selectedTileLayer < m_levelMap->GetTileLayerAmount())
				{
					float tilemapScale = 2.0f;
					auto tex = m_levelMap->GetTileLayer(m_selectedTileLayer).GetTileTexture();
					ImGui::ImageButton((ImTextureID)tex.id, { (float)tex.width * tilemapScale,(float)tex.height * tilemapScale }, { 0,0 }, { 1,1 }, 0, {0,0,0,0});
					ImVec2 tilePallateTopLeft = ImGui::GetItemRectMin();

					//Draw boarder around selected tile


					//Select a new Tile
					if (ImGui::IsItemHovered())
					{
						//Get mouse pos
						Vector2 tilePallateMousePos;

						tilePallateMousePos.x = ImGui::GetMousePos().x - tilePallateTopLeft.x;
						tilePallateMousePos.y = ImGui::GetMousePos().y - tilePallateTopLeft.y;

						tilePallateMousePos = Numbers::FloorMultiple(tilePallateMousePos, m_levelMap->TILE_SIZE * tilemapScale);

						ImVec2 recTL = { tilePallateTopLeft.x + tilePallateMousePos.x,tilePallateTopLeft.y + tilePallateMousePos.y };
						ImVec2 recBR = { recTL.x + m_levelMap->TILE_SIZE * tilemapScale,recTL.y + m_levelMap->TILE_SIZE * tilemapScale };

						ImDrawList* drawList = ImGui::GetWindowDrawList();
						drawList->AddRect(recTL,recBR,ImGui::GetColorU32(IM_COL32(255, 255, 255, 255)));

						//Select a tile
						if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
						{
							Vector2 toCells = {0,0};
							if (tilePallateMousePos.x != 0.0f) toCells.x = floor(tilePallateMousePos.x / (m_levelMap->TILE_SIZE * tilemapScale));
							if (tilePallateMousePos.y != 0.0f) toCells.y = floor(tilePallateMousePos.y / (m_levelMap->TILE_SIZE * tilemapScale));

							ImGui::Text( (std::to_string(toCells.x) + "," + std::to_string(toCells.y)).c_str());

							m_selectedTile = (int)((toCells.y * (tex.width/ m_levelMap->TILE_SIZE)) + toCells.x);
						}
					}
				}
				ImGui::Text(("Selected Tile" + std::to_string(m_selectedTile)).c_str());

				//Resize the map grid cells
				ImGui::BeginGroup();

				ImGui::Text("Tools");

				if (ImGui::Button("Pencil"))
				{
					m_tileState = TilePlacementState::Pencil;
				}

				if (ImGui::Button("Rectangle"))
				{
					m_tileState = TilePlacementState::Rectangle;
				}

				if (ImGui::Button("Fill"))
				{
					m_tileState = TilePlacementState::Fill;
				}

				ImGui::Checkbox("Auto Tile", &m_autoTile);

				ImGui::EndGroup();

				//Draw layers
				ImGui::BeginGroup();
				{
					if (ImGui::Button("New Tile Layer"))
					{
						m_openNewTileWindow = true;
					}

					for (int i = 0; i < m_levelMap->GetTileLayerAmount(); i++)
					{
						auto& layer = m_levelMap->GetTileLayer(i);

						auto tree = ImGui::TreeNode(layer.GetName().c_str());

						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left) && ImGui::IsItemHovered())
						{
							m_selectedTileLayer = i;
						}

						if (i == m_selectedTileLayer)
						{
							ImGui::SameLine();
							ImGui::Text(" Editing");
						}

						if (tree)
						{
							ImGui::Checkbox("Visible",&layer.m_visible);
							
							ImGui::TreePop();
						}
					}

					ImGui::EndGroup();
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Nodes"))
			{
				m_editorState = EditorStates::Nodes;

				if(ImGui::Button("Fill Empty cells with nodes"))
				{
					for (int xx = 0; xx < m_levelMap->GetWidth(); xx++)
					{
						for (int yy = 0; yy < m_levelMap->GetHeight(); yy++)
						{
							//if (m_levelMap->Get(xx, yy) == 0)
							//{
							//	Vector2 newPos = { xx * m_levelMap->TILE_SIZE,yy * m_levelMap->TILE_SIZE };
							//	newPos = Vector2Add(newPos, { m_levelMap->TILE_SIZE / 2.0f,m_levelMap->TILE_SIZE / 2.0f });
							//	Graph2D::Node* newNode = m_graph->AddNode(newPos);

							//	for (auto nearby : m_graph->GetNearbyNodes(newPos, 16))
							//	{
							//		m_graph->ConnectNodes(newNode, nearby, Vector2Distance(newNode->data, nearby->data));
							//	}
							//}
						}
					}
				}
				//Resize the map grid cells

				ImGui::EndTabItem();
			} 

			/// Layer Window ///////////////////
			if (m_openNewTileWindow)
			{
				ImGui::OpenPopup("OpenNewTileWindow");
				m_tilesetSelected = "tle_collision.png";
				m_openNewTileWindow = false;
			}

			//center this window when appearing
			ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
			ImGui::SetNextWindowPos(center, ImGuiCond_::ImGuiCond_Always, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("OpenNewTileWindow"))
			{
				ImGui::InputText("Layer Name", m_newTileName,32);

				if(ImGui::BeginCombo("Tileset Texture", m_tilesetSelected.c_str()))
				{
					for (auto const& tilesetString : m_tilesetsToSelect)
					{
						bool isSelected = (tilesetString == m_tilesetSelected);
						if (ImGui::Selectable(tilesetString.c_str(),isSelected))
						{
							m_tilesetSelected = tilesetString;
							isSelected = true;
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				if (ImGui::Button("Cancel"))
				{
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("Save"))
				{
					m_levelMap->AddTileLayer(TileLayer(m_newTileName, m_tilesetSelected.c_str(),m_app->GetResources()->GetTileset(m_tilesetSelected.c_str()),
						m_levelMap->GetWidth(),m_levelMap->GetHeight()));
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			/// Layer Window /////////////////

			//Game objects yah.
			if (ImGui::BeginTabItem("Game Objects"))
			{
				m_editorState = EditorStates::Entities;
				ImGui::ListBoxHeader("Rooms");
				{
					for (auto item : m_gameObjectIDsList)
					{
						ImGui::Selectable(item.name());
						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
						{
							if (m_entityToPlace == nullptr)
							{
								delete m_entityToPlace;
								m_entityToPlace = nullptr;
							}
							m_entityToPlace = m_objectFactory->CreateGameObject(item,this);
						}
					}

					ImGui::ListBoxFooter();
				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
	}

	//Map Window
	ImGui::Begin("Game Window");
	{
		ImGui::BeginChild("Level Batch", {0,0}, ImGuiWindowFlags_AlwaysHorizontalScrollbar);
		{
			auto rt = m_app->GetRenderTexture();
			
			ImVec2 gameImageSize = { (float)rt.texture.width * m_gameViewZoom,(float)rt.texture.height * m_gameViewZoom };
			//Center Render target
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - gameImageSize.x) * 0.5f);
			ImGui::SetCursorPosY((ImGui::GetWindowSize().y - gameImageSize.y) * 0.08f);

			ImGui::ImageButton((void*)rt.texture.id, gameImageSize, { 0,1 }, { 1,0 }, 0);
			
			//Get mouse pos
			ImVec2 editorTopLeft = ImGui::GetItemRectMin();

			m_mouseInGameWindow = ImGui::IsItemHovered();

			ImGui::SliderFloat("Zoom",&m_gameViewZoom,1,3,"",0.25f);

			m_gameWindowMousePos.x = ImGui::GetMousePos().x - editorTopLeft.x;
			m_gameWindowMousePos.y = ImGui::GetMousePos().y - editorTopLeft.y;

			ImGui::EndChild();
		}

		ImGui::End();
	}

	//EndDraw
	ImGui::Render();
	m_drawData = ImGui::GetDrawData();
	raylib_render_cimgui(m_drawData);
}

void LevelEditorState::UpdateRoomFilePaths()
{
	m_RoomFilePaths.clear();
	for (const auto& entry : std::filesystem::directory_iterator("Rooms"))
	{
		m_RoomFilePaths.push_back(entry.path());
	}
}

