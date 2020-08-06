#pragma once
#include "LevelState.h"
#include "Application.h"
#include "Agent.h"
#include "Path.h"
#include "raylib.h"
#include <vector>
#include <filesystem>
#include "ObjectFactory.h"

struct ImDrawData;

class Graph2D;
class Graph2DEditor;

class LevelEditorState : public LevelState
{
public:
	enum EditorStates {
		Tiles,
		Nodes,
		Entities
	};

	enum TilePlacementState {
		Pencil,
		Rectangle,
		Fill
	};

	LevelEditorState(Application* app);
	virtual ~LevelEditorState();
	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void EndDraw();

	void UpdateRoomFilePaths();

	void Save(std::string fileName);
	void Load(std::string fileName);
	
	Vector2 GetWorldMousePos();

	void FloodFillTiles(int x, int y, int value, int targetValue);

private:
	EditorStates m_editorState = EditorStates::Tiles;
	Vector2 m_gameWindowMousePos = {};

	ObjectFactory m_objectFactory;

	std::vector<std::filesystem::path> m_RoomFilePaths;
	std::vector<std::type_index> m_gameObjectIDsList;
	std::vector<GameObject*> m_gameObjects; // Sorted to proper location when game run.

	//GameWindow
	bool m_mouseInGameWindow;

	//Controls
		//Map Window
		int m_levelMapWidth;
		int m_levelMapHeight;

		//Tiles
		TilePlacementState m_tileState = TilePlacementState::Pencil;
		
		MouseButton m_placeTileValue = MouseButton::MOUSE_LEFT_BUTTON;
		Vector2 m_tileRectTopleft = { 0,0 };
		Vector2 m_tileRectBottemRight = {0,0};

	//Data
	char m_saveFileName[32] = "";

	bool m_loadMenuOpen = false;
	bool m_saveMenuOpen = false;
	int m_selectedLoadFile = 0;
	float m_gameViewZoom = 2;
	float* m_mapClearColour = new float[3];

	//Toggels
	bool m_drawGrid = true;
	bool m_drawNodes = true;
	bool m_snappedToGrid = false;

	//Navigation
	Vector2 m_panPosition = { 0,0 };

	//Graph
	Graph2DEditor* m_graphEditor = nullptr;
	Graph2D* m_graph = nullptr;
	
};