#include "LevelEditorState.h"
#include "Behaviour.h"
#include "raylib.h"
#include "FollowPathBehavior.h"
#include "Graph2D.h"
#include "Graph2DEditor.h"
#include "Numbers.h"
#include "Agent.h"
#include "FollowPathBehavior.h"

#include "raygui.h"
#include "ricons.h"
#include "tinyxml2.h"
#include "imgui.h"

LevelEditorState::LevelEditorState(Application* app) : LevelState::LevelState(app)
{
	m_graphEditor = new Graph2DEditor(this);
	m_graph = new Graph2D();

	m_graphEditor->SetGrapth(m_graph);

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

	//Controls
	//ImGui::ShowDemoWindow();

	//ImGui::Begin("Test Window");
	//ImGui::Button("Yeah");
	//ImGui::SetWindowPos({ 0,0 });
	//ImGui::SetWindowPos({10,10});
	//ImGui::End();
	//ImGui
	
	//ImGui

	if (IsKeyDown(KEY_C))
	{
		Rectangle buttonRect = { 8,8,48,16 };

		if (GuiButton(buttonRect, GuiIconText(RICON_BOX_GRID,"")))
		{
			m_editorState = EditorStates::Tiles;
		}
		buttonRect.y += 20;

		if (GuiButton(buttonRect, GuiIconText(RICON_CURSOR_HAND, "")))
		{
			m_editorState = EditorStates::Nodes;
		}
		buttonRect.y += 20;

		if (GuiButton(buttonRect, GuiIconText(RICON_PLAYER_PLAY, "")))
		{
			m_editorState = EditorStates::Entities;
		}
		buttonRect.y += 20;

		//Toggels
		Rectangle toggleRec = { 8,(float)m_app->GetGameHeight() - 20,16,16 };

		if (GuiButton(toggleRec, GuiIconText(RICON_GRID,"")))
		{
			m_drawGrid = !m_drawGrid;
		}
		toggleRec.x += 20;

		if (GuiButton(toggleRec, GuiIconText((m_drawNodes)? RICON_EYE_OFF : RICON_EYE_ON, "")))
		{
			m_drawNodes = !m_drawNodes;
		}
		toggleRec.x += 20;

		if (GuiButton(toggleRec, GuiIconText(RICON_FILE_SAVE, "")))
		{
			Save("");
		}
		toggleRec.x += 20;

		if (GuiButton(toggleRec, GuiIconText(RICON_FILE_OPEN, "")))
		{
			Load("");
		}
		toggleRec.x += 20;

		if (GuiButton(toggleRec, GuiIconText(RICON_BIN, "")))
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
		toggleRec.x += 20;
	}
	else
	{
		switch (m_editorState)
		{
			case EditorStates::Tiles:
			{
				Vector2 gridPos = m_levelMap->ToGridPos(m_app->GetScaledMousePos());

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

	level.SaveFile("data.xml");
}

void LevelEditorState::Load(std::string fileName)
{
	tinyxml2::XMLDocument level;

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