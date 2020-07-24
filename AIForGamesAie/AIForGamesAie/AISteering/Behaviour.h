#pragma once

class Agent;

class Behaviour
{
public:

	Behaviour() {};
	virtual ~Behaviour() {};

	virtual void Update(Agent* obj, float deltaTime) {};
	virtual void Draw(Agent* obj) {};
};

