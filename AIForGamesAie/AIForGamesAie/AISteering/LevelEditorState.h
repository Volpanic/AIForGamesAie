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

	void Save(std::string fileName);
	void Load(std::string fileName);
	
	Vector2 EditorMousePos();

private:
	EditorStates m_editorState = EditorStates::Tiles;

	bool m_drawGrid = true;
	bool m_drawNodes = true;
	bool m_snappedToGrid = false;

	Graph2DEditor* m_graphEditor = nullptr;
	Graph2D* m_graph = nullptr;
	ImDrawData* m_drawData;
};