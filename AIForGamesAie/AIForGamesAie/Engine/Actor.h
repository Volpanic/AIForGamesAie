#pragma once
#include "GameObject.h"
#include "Drawable.h"
#include "LevelState.h"

class Actor : public GameObject
{
public:
	
	Actor(LevelState* level);
	virtual ~Actor();
	virtual std::type_index GetCategory() { return typeid(Actor); };
	virtual void Update(float deltaTime);
	virtual void Draw();

	virtual void MoveX(float x);
	virtual void MoveY(float y);

protected:
	Vector2 m_velocity = { 0,0 };
	LevelState* m_level;

private:
	Vector2 m_subPixelMovement = { 0,0 };
	bool m_solid = true;
};

