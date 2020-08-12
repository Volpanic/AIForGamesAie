#pragma once
#include "Agent.h"

class DarkBlueShark : public Agent
{
public:

	enum SharkState
	{
		Movement,
		Attack
	};


	DarkBlueShark(LevelState* level);
	virtual ~DarkBlueShark();

	virtual std::type_index GetCategory() { return typeid(Agent); };
	virtual std::string GetType() { return "DarkBlueShark"; };

	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void DrawInEditor(const Vector2& mousePos);

protected:
private:

	float m_checkCanSeePlayerTimer = 0;
	DarkBlueShark::SharkState m_currentState = SharkState::Movement;
};


