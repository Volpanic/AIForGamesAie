#pragma once
#include "Agent.h"

class BrownShark : public Agent
{
public:

	enum SharkState
	{
		Movement,
		Attack
	};


	BrownShark(LevelState* level);
	virtual ~BrownShark();

	virtual std::type_index GetCategory() { return typeid(Agent); };
	virtual std::string GetType() { return "BrownShark"; };

	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void DrawInEditor(const Vector2& mousePos);

protected:
private:

	float m_checkCanSeePlayerTimer = 0;
	BrownShark::SharkState m_currentState = SharkState::Movement;
};

