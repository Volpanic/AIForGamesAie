#pragma once
#include "Agent.h"

class DarkBlueShark : public Agent
{
public:

	DarkBlueShark(LevelState* level);
	virtual ~DarkBlueShark();
	virtual std::type_index GetCategory() { return typeid(Agent); };
	virtual void Update(float deltaTime);
	virtual void Draw();

protected:
private:
};


