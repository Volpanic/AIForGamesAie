#pragma once
#include "IGameState.h"

class GameOverState : public IGameState
{
public:
	GameOverState(Application* app);
	virtual ~GameOverState();

	virtual void Load();
	virtual void Unload();

	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void EndDraw();

private:
	bool m_hasInputed = false;
};

