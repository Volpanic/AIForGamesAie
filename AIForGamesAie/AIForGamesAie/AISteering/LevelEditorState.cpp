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

	Load("");
	
}

LevelEditorState::~LevelEditorState()
{
	delete m_graphEditor;
	delete m_graph;

	//delete m_drawData;
}

Vector2 LevelEditorState::EditorMousePos()
{
	Vector2 pos = m_app->GetScaledMousePos();
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
	if (ImGui::GetIO().WantCaptureMouse)
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
		m_camera.target.x = m_panPosition.x - GetMousePosition().x;
		m_camera.target.y = m_panPosition.y - GetMousePosition().y;
	}

	if (m_graphEditor->m_path != nullptr)
	{
		auto testAgent = Add<Agent>(new Agent(this));
		m_graphEditor->m_path->SetPathType(PathType::Open);
		testAgent->SetPosition(m_graphEditor->m_selectedNode->data.x, m_graphEditor->m_selectedNode->data.y);
		testAgent->SetBehaviour(new FollowPathBehavior(m_graphEditor->m_path,250.0f));
		m_graphEditor->m_path = nullptr;
	}

	if (IsKeyDown(KEY_C))
	{
		return;
	}

	m_snappedToGrid = IsKeyDown(KEY_LEFT_CONTROL);

	switch (m_editorState)
	{
		case EditorStates::Tiles:
		{
			Vector2 gridPos = m_levelMap->ToGridPos(EditorMousePos());

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
			m_graphEditor->Update(EditorMousePos(),deltaTime);
			break;
		}
	}

	LevelState::Update(deltaTime);
}

void LevelEditorState::Draw()
{
	BeginMode2D(m_camera);
	ClearBackground(WHITE);

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

	ClearBackground(BLACK);

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
		ImGui::EndMainMenuBar();
	}

	//Save Menu
	if (m_saveMenuOpen)
	{
		ImGui::Begin("Save Options");

		ImGui::InputText("File Name: Rooms/",m_saveFileName,IM_ARRAYSIZE(m_saveFileName));

		if (ImGui::Button("Save"))
		{
			Save(m_saveFileName);
			m_saveMenuOpen = false;
		}

		ImGui::End();
	}

	if (m_loadMenuOpen)
	{
		ImGui::Begin("Load Options");

		//ImGui::ListBox("Rooms",&m_selectedLoadFile,)

		ImGui::End();
	}

	if (IsKeyDown(KEY_C))
	{
		//Controls

		ImGui::Begin("Controls Window");
		{
			if (ImGui::Button("Tiles"))
			{
				m_editorState = EditorStates::Tiles;
			}

			if (ImGui::Button("Nodes"))
			{
				m_editorState = EditorStates::Nodes;
			}

			if (ImGui::Button("Entities"))
			{
				m_editorState = EditorStates::Entities;
			}

			ImGui::Text("Toggels");
			ImGui::Checkbox("Draw Grid", &m_drawGrid);
			ImGui::Checkbox("Draw Nodes", &m_drawNodes);

			ImGui::Text("Commands");

			if (ImGui::Button("Save"))
			{
				Save("");
			}

			if (ImGui::Button("Load"))
			{
				Load("");
			}

			if (ImGui::Button("Clear"))
			{
				Graph2D* graph = new Graph2D();
				delete m_graph;
				m_graph = graph;
				m_graphEditor->SetGrapth(graph);

				for (int xx = 0; xx < m_levelMap->GetWidth(); xx++)
				{
					for (int yy = 0; yy < m_levelMap->GetHeight(); yy++)
					{
						m_levelMap->Set(xx, yy, 0);
					}
				}
			}
		}
		ImGui::End();
		
	}
	else
	{
		switch (m_editorState)
		{
			case EditorStates::Tiles:
			{
				Vector2 gridPos = m_levelMap->ToGridPos(EditorMousePos());

				DrawRectangleLinesEx({gridPos.x * m_levelMap->TILE_SIZE,gridPos.y * m_levelMap->TILE_SIZE,(float)m_levelMap->TILE_SIZE ,(float)m_levelMap->TILE_SIZE },1,LIGHTGRAY);

				break;
			}

			case EditorStates::Nodes:
			{
				m_graphEditor->Draw();
				break;
			}
		}
	}

	LevelState::Draw();

	EndMode2D();
}

void LevelEditorState::EndDraw()
{
	ImGui::ShowMetricsWindow();
	ImGui::ShowStyleEditor();
	ImGui::ShowDemoWindow();

	ImGui::Begin("Game Window");
	{
		ImGui::BeginChild("Level Batch");
		{
			ImGui::Image((void*)m_app->GetRenderTexture().texture.id, { (float)m_app->GetGameWidth(),(float)m_app->GetGameHeight()}, { 0,1 }, { 1,0 });

			ImGui::EndChild();
		}

		ImGui::End();
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

	level.LoadFile("data.xml");

	tinyxml2::XMLNode* pRoot = level.FirstChild();


	//Load Tiles
		{
			tinyxml2::XMLElement* pMap = pRoot->FirstChildElement("Map");

			int mWidth = 1;
			int mHeight = 1;

			pMap->QueryIntAttribute("Width", &mWidth);
			pMap->QueryIntAttribute("Height", &mHeight);

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