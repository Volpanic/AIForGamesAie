#pragma once
#include "LevelState.h"
#include "Application.h"
#include "Agent.h"
#include "Path.h"
#include "raylib.h"

struct ImDrawData;

class Graph2D;
class Graph2DEditor;

enum EditorStates {
	Tiles,
	Nodes,
	Entities
};

class LevelEditorState : public LevelState
{
public:
	LevelEditorState(Application* app);
	virtual ~LevelEditorState();
	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void EndDraw();

	void Save(std::string fileName);
	void Load(std::string fileName);
	
	Vector2 EditorMousePos();

private:
	EditorStates m_editorState = EditorStates::Tiles;

	//Data
	char m_saveFileName[32] = "";

	bool m_loadMenuOpen = false;
	bool m_saveMenuOpen = false;
	int m_selectedLoadFile = 0;

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