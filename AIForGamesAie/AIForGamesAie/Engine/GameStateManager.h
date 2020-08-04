#pragma once
#include <map>
#include <list>
#include <functional>
#include "IGameState.h"

class StateTransition;

class GameStateManager
{
public:
	GameStateManager();
	~GameStateManager();

	void Update(float deltaTime);
	void Draw();
	void EndDraw();

	void SetState(const char* name, IGameState* state);
	IGameState* GetState(const char* name);
	void PushState(const char* name);
	void PushState(const char* name, StateTransition* transition);
	void PopState();

protected:
	std::map<const char*, IGameState*> m_states;
	std::list<IGameState*> m_stateStack;
	std::list<std::function<void()>> m_commands;

private:
	StateTransition* m_transition = nullptr;
	
};