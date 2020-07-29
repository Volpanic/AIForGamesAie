#include "LevelEditorState.h"
#include "Behaviour.h"
#include "raylib.h"
#include "FollowPathBehavior.h"
#include "Graph2D.h"
#include "Graph2DEditor.h"

#include "raygui.h"
#include "ricons.h"
#include "tinyxml2.h"

LevelEditorState::LevelEditorState(Application* app) : LevelState::LevelState(app)
{
	m_graphEditor = new Graph2DEditor();
	m_graph = new Graph2D();

	m_graphEditor->SetGrapth(m_graph);

	Load("");
}

LevelEditorState::~LevelEditorState()
{
	delete m_graphEditor;
	delete m_graph;
}

Vector2 LevelEditorState::EditorMousePos()
{
	Vector2 pos = m_app->GetScaledMousePos();
	pos.x = floor(pos.x);
	pos.y = floor(pos.y);

	if (m_snappedToGrid)
	{
		Vector2 gridPos = m_levelMap->ToGridPos(pos);
		return Vector2Scale(gridPos,m_levelMap->TILE_SIZE);
	}
	else
	{
		return pos;
	}
}

void LevelEditorState::Update(float deltaTime)
{
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

	//LevelState::Update(deltaTime);
}

void LevelEditorState::Draw()
{
	ClearBackground(WHITE);

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

	LevelState::Draw();
	m_levelMap->Draw();

	if (m_drawNodes && m_editorState != EditorStates::Nodes)
	{
		m_graphEditor->DrawOnlyNodes();
	}

	//Controls
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
		Rectangle toggleRec = { 8,m_app->GetGameHeight() - 20,16,16 };

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
}

void LevelEditorState::Save(std::string fileName)
{
	tinyxml2::XMLDocument level;

	//Root
	tinyxml2::XMLNode* pRoot = level.NewElement("Root");
	level.InsertFirstChild(pRoot);

		//Map Tiles
		{
			tinyxml2::XMLNode* pMap = level.NewElement("Map");
			pRoot->InsertFirstChild(pMap);

			tinyxml2::XMLElement* gridSize = level.NewElement("MapGridSize");
			gridSize->SetAttribute("Width", m_levelMap->GetWidth());
			gridSize->SetAttribute("Height", m_levelMap->GetHeight());

			tinyxml2::XMLElement* gridData = level.NewElement("MapData");

			//Write tile data
			for (int i = 0; i < m_levelMap->GetSize(); i++)
			{
				tinyxml2::XMLElement* gridDataElement = level.NewElement("Tile");
				gridDataElement->SetText(m_levelMap->Get(i));

				gridData->InsertEndChild(gridDataElement);
			}

			pMap->InsertEndChild(gridSize);
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
				tinyxml2::XMLElement* nodeElement = level.NewElement("Node");
				nodeElement->SetText((char*)(&node));

				nodeData->InsertEndChild(nodeElement);
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
			tinyxml2::XMLNode* pMap = pRoot->FirstChildElement("Map");

			int mWidth = 1;
			int mHeight = 1;

			tinyxml2::XMLElement* mapGridSize = pMap->FirstChildElement("MapGridSize");

			mapGridSize->QueryIntAttribute("Width", &mWidth);
			mapGridSize->QueryIntAttribute("Height", &mHeight);

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
			tinyxml2::XMLNode* pMap = pRoot->FirstChildElement("Nodes");

			tinyxml2::XMLElement* mapData = pMap->FirstChildElement("NodeData");
			tinyxml2::XMLElement* mapDataListElement = mapData->FirstChildElement("Node");

			std::vector<Graph2D::Node*> nodes;
			while (mapDataListElement != nullptr)
			{
				const char* tileValue = mapDataListElement->Attribute("Node");

				nodes.push_back(const_cast<Graph2D::Node*>(reinterpret_cast<const Graph2D::Node*>(tileValue)));

				mapDataListElement = mapDataListElement->NextSiblingElement("Node");
			}

			Graph2D *gra = new Graph2D(nodes);
			delete m_graphEditor->GetGraph();
			m_graphEditor->SetGrapth(gra);
		}
}