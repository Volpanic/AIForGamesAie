#pragma once
#include "Agent.h"
#include "Clam.h"
#include <vector>

class PinkOrca : public Agent
{
public:

	enum SharkState
	{
		Movement,
		Attack
	};


	PinkOrca(LevelState* level);
	virtual ~PinkOrca();

	void RefreshNearestClam(int pos);

	virtual std::type_index GetCategory() { return typeid(Agent); };
	virtual std::string GetType() { return "PinkOrca"; };

	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void DrawInEditor(const Vector2& mousePos);

protected:
private:

	float m_checkCanSeePlayerTimer = 0;
	std::vector<Clam*> m_targetClams;
	int m_targetClamIndex = 0;
	int m_targetClamDirection = 0;
	bool m_firstRun = true;
	PinkOrca::SharkState m_currentState = PinkOrca::Movement;
};

