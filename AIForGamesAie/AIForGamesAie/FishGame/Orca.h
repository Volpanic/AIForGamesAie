#pragma once
#include "Agent.h"
#include "Clam.h"

class Orca : public Agent
{
public:

	enum SharkState
	{
		Movement,
		Attack
	};


	Orca(LevelState* level);
	virtual ~Orca();

	virtual std::type_index GetCategory() { return typeid(Agent); };
	virtual std::string GetType() { return "Orca"; };

	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void DrawInEditor(const Vector2& mousePos);

protected:
private:

	float m_checkCanSeePlayerTimer = 0;
	Clam* m_targetClam = nullptr;
	Orca::SharkState m_currentState = Orca::Movement;
};

