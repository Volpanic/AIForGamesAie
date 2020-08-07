#include "LevelState.h"
#include "raylib.h"
#include "raymath.h"
#include "Application.h"
#include "GameStateManager.h"
#include <iostream>
#include "GameObject.h"
#include "Actor.h"
#include "GameObject.h"
#include "tinyxml2.h"
#include "GameObjectFactory.h"
#include "ObjectTracker.h"


LevelState::LevelState(Application* app) : IGameState::IGameState(app)
{
	m_objectTracker = new ObjectTracker();
	m_camera.zoom = 1;
}

LevelState::LevelState(Application* app, std::string loadFileName,GameObjectFactory* factory) : IGameState::IGameState(app)
{
	m_objectTracker = new ObjectTracker();
	m_camera.zoom = 1;

	if (!LoadMap(loadFileName, factory))
	{
		std::cout << "Failed to load " + loadFileName << std::endl;
	}
}

LevelState::~LevelState()
{
	delete m_levelMap;
	delete m_objectTracker;
}

void LevelState::Load()
{

}

void LevelState::Unload()
{

}

void LevelState::Update(float deltaTime)
{
	m_objectTracker->Update(deltaTime);
}

void LevelState::Draw()
{
	BeginMode2D(m_camera);

	Color clear = { (unsigned char)(m_mapClearColour[0] * 255.0f),(unsigned char)(m_mapClearColour[1] * 255.0f),(unsigned char)(m_mapClearColour[2] * 255.0f),255 };
	ClearBackground(clear);

	m_levelMap->Draw();
	m_objectTracker->Draw();

	EndMode2D();
}

void LevelState::SetGraph(Graph2D* newGraph)
{
	if (m_graph != nullptr)
	{
		delete m_graph;
	}
	m_graph = newGraph;
}

void LevelState::SetMap(LevelMap* newMap)
{
	if (m_levelMap != nullptr)
	{
		delete m_levelMap;
	}
	m_levelMap = newMap;
}

void LevelState::SaveMap(std::string fileName)
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
		for (auto const& node : GetGraph()->GetNodes())
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

	//Entities :; 
	{
		tinyxml2::XMLNode* pEntity = level.NewElement("Entities");
		pRoot->InsertFirstChild(pEntity);

		tinyxml2::XMLElement* entityData = level.NewElement("EntityData");

		//Write entity data, using each objects sav function.
		m_objectTracker->ForEachObject([&](GameObject* object) 
		{

			tinyxml2::XMLElement* entityEntry = level.NewElement("EntityEntry");

			object->Save(&level, entityEntry);

			entityData->InsertEndChild(entityEntry);
		});

		pEntity->InsertEndChild(entityData);

		pRoot->InsertEndChild(pEntity);
	}

	level.SaveFile(("Rooms\\" + fileName + ".xml").c_str());
}

bool LevelState::LoadMap(std::string fileName, GameObjectFactory* factory)
{
	tinyxml2::XMLDocument level;

	if (!FileExists(fileName.c_str()))
	{
		return false;
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
		std::vector<std::vector<std::tuple<Vector2, float>>> otherConnections;

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
				Vector2 otherPos = { 0,0 };
				float weight = 0;
				connectionListElement->QueryFloatAttribute("OtherPositionX", &otherPos.x);
				connectionListElement->QueryFloatAttribute("OtherPositionY", &otherPos.y);
				connectionListElement->QueryFloatAttribute("Weight", &weight);

				//Enter data
				(otherConnections[nodeListPos]).push_back(std::make_tuple(otherPos, weight));

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
						newGraph->AddEdge(node, nodeOth, std::get<1>(connection));
						continue;
					}
				}
			}

			nodeListPos++;
		}

		//SetGraph
		delete m_graph;
		m_graph = newGraph;
	}

	//Load Entitites
	{
		tinyxml2::XMLNode* pEntities = pRoot->FirstChildElement("Entities");
		tinyxml2::XMLElement* entityData = pEntities->FirstChildElement("EntityData");
		tinyxml2::XMLElement* entityListElement = entityData->FirstChildElement("EntityEntry");

		m_objectTracker->Clear();

		while (entityListElement != nullptr)
		{
			//Load Position
			const char* entityName;

			entityListElement->QueryAttribute("Type", &entityName);

			GameObject* newOne = factory->CreateGameObject(entityName, this);
			newOne->Load(&level, entityListElement);

			m_objectTracker->Add(newOne->GetCategory(),newOne);

			entityListElement = entityListElement->NextSiblingElement("EntityEntry");
		}
	}
}