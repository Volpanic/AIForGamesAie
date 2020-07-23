#pragma once
#include <map>
#include <list>
#include <functional>
#include "IGameState.h"

class GameStateManager
{
public:
	GameStateManager();
	~GameStateManager();

	void Update(float deltaTime);
	void Draw();

	void SetState(const char* name, IGameState* state);
	IGameState* GetState(const char* name);
	void PushState(const char* name);
	void PopState();

protected:
	std::map<const char*, IGameState*> m_states;
	std::list<IGameState*> m_stateStack;
	std::list<std::function<void()>> m_commands;

private:
	
};