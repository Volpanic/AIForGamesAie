#pragma once
#include "GameObject.h"
#include "Drawable.h"

class PlayerTest : public GameObject
{
public:
	PlayerTest();
	virtual ~PlayerTest();

	virtual void Update(float deltaTime);

	virtual void Draw();

private:
	ComponentPtr drawable;
	Texture2D sprite;
};

