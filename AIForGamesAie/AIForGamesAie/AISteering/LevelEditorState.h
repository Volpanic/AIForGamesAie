#pragma once
#include "LevelState.h"
#include "Application.h"
#include "Path.h"
#include "raylib.h"
#include <vector>
#include <filesystem>
#include "GameObject.h"

struct ImDrawData;

class ObjectFactory;
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
	
	Vector2 GetWorldMousePos();

	void FloodFillTiles(int x, int y, int value, int targetValue);

private:
	EditorStates m_editorState = EditorStates::Tiles;
	Vector2 m_gameWindowMousePos = {};

	ObjectFactory* m_objectFactory = nullptr;

	std::vector<std::filesystem::path> m_RoomFilePaths;
	std::vector<std::type_index> m_gameObjectIDsList;

	//GameWindow
	bool m_mouseInGameWindow;

	//Controls
		//Map Window
		int m_levelMapWidth;
		int m_levelMapHeight;

		//Tiles
		TilePlacementState m_tileState = TilePlacementState::Pencil;

		std::vector<std::string> m_tilesetsToSelect;
		int m_selectedTile = 1;
		int m_selectedTileLayer = 0;
		
		MouseButton m_placeTileValue = MouseButton::MOUSE_LEFT_BUTTON;
		Vector2 m_tileRectTopleft = { 0,0 };
		Vector2 m_tileRectBottemRight = {0,0};

		//Entites
		GameObject* m_selectedEntity = nullptr;
		GameObject* m_entityToPlace = nullptr;

	//Data
	char m_saveFileName[32] = "";

	bool m_loadMenuOpen = false;
	bool m_runMenuOpen = false;
	bool m_saveMenuOpen = false;
	int m_selectedLoadFile = 0;
	float m_gameViewZoom = 2;

	//Toggels
	bool m_drawGrid = true;
	bool m_drawScreenGrid = true;
	bool m_drawNodes = true;
	bool m_snappedToGrid = false;

	//Navigation
	Vector2 m_panPosition = { 0,0 };

	//Graph
	Graph2DEditor* m_graphEditor = nullptr;
	Graph2D* m_graph = nullptr;
	
};