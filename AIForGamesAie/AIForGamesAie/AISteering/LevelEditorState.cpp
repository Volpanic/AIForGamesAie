#include "LevelEditorState.h"
#include "Behaviour.h"
#include "raylib.h"
#include "FollowPathBehavior.h"
#include "Graph2D.h"
#include "Graph2DEditor.h"
#include "Numbers.h"
#include "Agent.h"
#include "FollowPathBehavior.h"
#include "rlgl.h"

#include "raygui.h"
#include "ricons.h"
#include "tinyxml2.h"



#include "imgui.h"

LevelEditorState::LevelEditorState(Application* app) : LevelState::LevelState(app)
{
	m_graphEditor = new Graph2DEditor(this);
	m_graph = new Graph2D();

	m_graphEditor->SetGrapth(m_graph);
	m_camera.zoom = 1;

	m_mapClearColour[0] = 0;
	m_mapClearColour[1] = 0;
	m_mapClearColour[2] = 0;

	UpdateRoomFilePaths();
	m_objectFactory.GetAllGameObjectTypes(m_gameObjectIDsList);

	m_levelMapWidth = m_levelMap->GetWidth();
	m_levelMapHeight = m_levelMap->GetHeight();
}

LevelEditorState::~LevelEditorState()
{
	delete m_graphEditor;
	delete m_graph;
	delete m_mapClearColour;

	for (auto const object : m_gameObjects)
	{
		delete object;
	}
	m_gameObjects.clear();
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

void LevelEditorState::Update(float deltaTime)
{
	LevelState::Update(deltaTime);

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

			//Create
			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
			{
				m_levelMap->Set((int)gridPos.x, (int)gridPos.y, 1);
			}

			//Delete
			if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
			{
				m_levelMap->Set((int)gridPos.x, (int)gridPos.y, 0);
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
	BeginMode2D(m_camera);
	Color clear = { (unsigned char)(m_mapClearColour[0]*255.0f),(unsigned char)(m_mapClearColour[1] * 255.0f),(unsigned char)(m_mapClearColour[2]*255.0f),255 };
	ClearBackground(clear);

	if (m_drawNodes)
	{
		m_graphEditor->DrawOnlyNodes();
	}

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

	m_levelMap->Draw();

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
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::Text("Toggels");
			ImGui::Checkbox("Draw Grid", &m_drawGrid);
			ImGui::Checkbox("Draw Nodes", &m_drawNodes);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (m_saveMenuOpen)
	{
		ImGui::OpenPopup("SaveMenu");
		m_saveMenuOpen = false;
	}

	if (m_loadMenuOpen)
	{
		ImGui::OpenPopup("LoadMenu");
		m_loadMenuOpen = false;
	}

	//Save Menu
	// Always center this window when appearing
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
			Save(m_saveFileName);
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

	// Always center this window when appearing
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
					m_loadMenuOpen = false;
					Load(item.generic_string());
					ImGui::CloseCurrentPopup();
					break;
				}
			}

			ImGui::ListBoxFooter();
		}

		ImGui::SameLine();
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	switch (m_editorState)
	{
		case EditorStates::Tiles:
		{
			Vector2 gridPos = m_levelMap->ToGridPos(GetWorldMousePos());

			DrawRectangleLinesEx({gridPos.x * m_levelMap->TILE_SIZE,gridPos.y * m_levelMap->TILE_SIZE,(float)m_levelMap->TILE_SIZE ,(float)m_levelMap->TILE_SIZE },1,LIGHTGRAY);

			break;
		}

		case EditorStates::Nodes:
		{
			m_graphEditor->Draw();
			break;
		}
	}
	

	LevelState::Draw();

	EndMode2D();
}

void LevelEditorState::EndDraw()
{
	DrawRectangle(0,0,GetScreenWidth(),GetScreenHeight(),BLACK);

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

				ImGui::EndTabItem();
			}

			if(ImGui::BeginTabItem("Tiles"))
			{
				m_editorState = EditorStates::Tiles;

				//Resize the map grid cells
				ImGui::BeginChild("Tools");

				ImGui::Text("Tools");

				if (ImGui::Button("Pencil"))
				{

				}

				if (ImGui::Button("Rectangle"))
				{

				}

				if (ImGui::Button("Fill"))
				{

				}

				ImGui::EndChild();

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Nodes"))
			{
				m_editorState = EditorStates::Nodes;

				if(ImGui::Button("Run Path Test"))
				{
					if (m_graphEditor->m_path != nullptr)
					{
						auto testAgent = Add<Agent>(new Agent(this));
						m_graphEditor->m_path->SetPathType(PathType::Open);
						testAgent->SetPosition(m_graphEditor->m_selectedNode->data.x, m_graphEditor->m_selectedNode->data.y);
						testAgent->SetBehaviour(new FollowPathBehavior(m_graphEditor->m_path, 250.0f));
						m_graphEditor->m_path = nullptr;
					}
				}

				if (ImGui::Button("Clear Test Agents"))
				{

				}
				//Resize the map grid cells

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Game Objects"))
			{
				ImGui::ListBoxHeader("Rooms");
				{
					for (auto item : m_gameObjectIDsList)
					{
						ImGui::Selectable(item.name());
						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
						{
							//m_loadMenuOpen = false;
							//Load(item.generic_string());
							//break;
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
			ImGui::ImageButton((void*)m_app->GetRenderTexture().texture.id, { (float)m_app->GetGameWidth() * m_gameViewZoom,(float)m_app->GetGameHeight() * m_gameViewZoom }, { 0,1 }, { 1,0 },0);
			
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
}

void LevelEditorState::UpdateRoomFilePaths()
{
	m_RoomFilePaths.clear();
	for (const auto& entry : std::filesystem::directory_iterator("Rooms"))
	{
		m_RoomFilePaths.push_back(entry.path());
	}
}

void LevelEditorState::Save(std::string fileName)
{
	tinyxml2::XMLDocument level;

	//Root
	tinyxml2::XMLNode* pRoot = level.NewElement("Root");
	level.InsertFirstChild(pRoot);

		//Map Tiles
		{
			tinyxml2::XMLElement* pMap = level.NewElement("Map");

			pMap->SetAttribute("Width", m_levelMap->GetWidth());
			pMap->SetAttribute("Height", m_levelMap->GetHeight());

			pMap->SetAttribute("ClearR", m_mapClearColour[0]);
			pMap->SetAttribute("ClearG", m_mapClearColour[1]);
			pMap->SetAttribute("ClearB", m_mapClearColour[2]);

			tinyxml2::XMLElement* gridData = level.NewElement("MapData");

			//Write tile data
			for (int i = 0; i < m_levelMap->GetSize(); i++)
			{
				tinyxml2::XMLElement* gridDataElement = level.NewElement("Tile");
				gridDataElement->SetText(m_levelMap->Get(i));

				gridData->InsertEndChild(gridDataElement);
			}

			pMap->InsertEndChild(gridData);

			pRoot->InsertEndChild(pMap);
		}

		//NODES 
		{
			tinyxml2::XMLNode* pNodes = level.NewElement("Nodes");
			pRoot->InsertFirstChild(pNodes);

			tinyxml2::XMLElement* nodeData = level.NewElement("NodeData");

			//Write node data
			for (auto const& node : m_graphEditor->GetGraph()->GetNodes())
			{
				tinyxml2::XMLElement* nodeEntry = level.NewElement("NodeEntry");

				{
					//NodeData
					nodeEntry->SetAttribute("PositionX", node->data.x);
					nodeEntry->SetAttribute("PositionY", node->data.y);

					//Node Connections
					for (auto const& connect : node->connections)
					{
						
						tinyxml2::XMLElement* nodeConnection = level.NewElement("NodeConnection");
						nodeConnection->SetAttribute("OtherPositionX", connect.to->data.x);
						nodeConnection->SetAttribute("OtherPositionY", connect.to->data.y);
						nodeConnection->SetAttribute("Weight", connect.data);

						nodeEntry->InsertEndChild(nodeConnection);
					}
				}

				nodeData->InsertEndChild(nodeEntry);
			}

			pNodes->InsertEndChild(nodeData);

			pRoot->InsertEndChild(pNodes);
		}

	level.SaveFile(("Rooms\\" + fileName + ".xml").c_str());
}

void LevelEditorState::Load(std::string fileName)
{
	tinyxml2::XMLDocument level;

	if (!FileExists(fileName.c_str()))
	{
		return;
	}

	level.LoadFile(fileName.c_str());

	tinyxml2::XMLNode* pRoot = level.FirstChild();


	//Load Tiles
		{
			tinyxml2::XMLElement* pMap = pRoot->FirstChildElement("Map");

			int mWidth = 1;
			int mHeight = 1;

			pMap->QueryIntAttribute("Width", &mWidth);
			pMap->QueryIntAttribute("Height", &mHeight);

			float mClearR = 255;
			float mClearG = 255;
			float mClearB = 255;

			pMap->QueryFloatAttribute("ClearR", &mClearR);
			pMap->QueryFloatAttribute("ClearG", &mClearG);
			pMap->QueryFloatAttribute("ClearB", &mClearB);

			m_mapClearColour[0] = mClearR;
			m_mapClearColour[1] = mClearG;
			m_mapClearColour[2] = mClearB;

			std::cout << mWidth << " : " << mHeight << std::endl;

			tinyxml2::XMLElement* mapData = pMap->FirstChildElement("MapData");
			tinyxml2::XMLElement* mapDataListElement = mapData->FirstChildElement("Tile");

			LevelMap* newerMap = new LevelMap(mWidth, mHeight);

			int pos = 0;
			while (mapDataListElement != nullptr)
			{
				int tileValue = 0;
				mapDataListElement->QueryIntText(&tileValue);
				newerMap->Set(pos, tileValue);
				//std::cout << tileValue << std::endl;

				mapDataListElement = mapDataListElement->NextSiblingElement("Tile");

				pos++;
			}

			delete m_levelMap;
			m_levelMap = newerMap;
		}

		//Load Nodes
		{
			tinyxml2::XMLNode* pNodes = pRoot->FirstChildElement("Nodes");
			tinyxml2::XMLElement* nodeData = pNodes->FirstChildElement("NodeData");
			tinyxml2::XMLElement* nodeListElement = nodeData->FirstChildElement("NodeEntry");
			
			std::vector<Graph2D::Node*> newNodes;

			//Kinda Gross
			std::vector<std::vector<std::tuple<Vector2,float>>> otherConnections;

			int nodeListPos = 0;
			//Load all nodes into Place
			while (nodeListElement != nullptr)
			{
				//Load Position
				Vector2 pos;
				nodeListElement->QueryFloatAttribute("PositionX", &pos.x);
				nodeListElement->QueryFloatAttribute("PositionY", &pos.y);
				
				//AddNode to vector
				Graph2D::Node* newNode = new Graph2D::Node();
				newNode->data = pos;
				newNodes.push_back(newNode);

				//Connections
				tinyxml2::XMLElement* connectionListElement = nodeListElement->FirstChildElement("NodeConnection");

				otherConnections.push_back(std::vector<std::tuple<Vector2, float>>());
				//AddConnections
				while (connectionListElement != nullptr)
				{
					//Get data
					Vector2 otherPos = {0,0};
					float weight = 0;
					connectionListElement->QueryFloatAttribute("OtherPositionX", &otherPos.x);
					connectionListElement->QueryFloatAttribute("OtherPositionY", &otherPos.y);
					connectionListElement->QueryFloatAttribute("Weight", &weight);

					//Enter data
					(otherConnections[nodeListPos]).push_back(std::make_tuple(otherPos,weight));

					connectionListElement = connectionListElement->NextSiblingElement("NodeConnection");
				}

				//Next
				nodeListPos++;
				nodeListElement = nodeListElement->NextSiblingElement("NodeEntry");
			}

			
			Graph2D* newGraph = new Graph2D(newNodes);
			//Connect nodes
			nodeListPos = 0;
			for (auto const& node : newNodes)
			{
				//Loop through connections list
				for (auto const& connection : otherConnections[nodeListPos])
				{
					//Check if should connect nodes
					Vector2 otherPos = connection._Myfirst._Val;
					for (auto const& nodeOth : newNodes)
					{
						if ((nodeOth->data.x == otherPos.x) && (nodeOth->data.y == otherPos.y))
						{
							newGraph->AddEdge(node,nodeOth,std::get<1>(connection));
							continue;
						}
					}
				}

				nodeListPos++;
			}

			LevelState::Draw();

			//SetGraph
			delete m_graph;
			m_graph = newGraph;
			m_graphEditor->SetGrapth(m_graph);
		}
}