#pragma once
#include "GameObject.h"
#include "Drawable.h"

class Actor : public GameObject
{
public:
	Actor();
	virtual ~Actor();
	virtual void Update(float deltaTime);
	virtual void Draw();

	virtual void MoveX(float x);
	virtual void MoveY(float y);

protected:

private:
	Vector2 m_velocity = {0,0};
	bool m_solid = true;
	Drawable* drawable;
};

