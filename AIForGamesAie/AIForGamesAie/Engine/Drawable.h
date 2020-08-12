#pragma once
#include "raylib.h"
#include "raymath.h"
#include "GameObject.h"

class Drawable : public Component
{
public:
	Drawable(Texture2D sprite, int width, int height);
	void SetDrawRect(int frame);
	virtual void Update(GameObject* gameObject, float deltaTime);
	virtual void Draw(GameObject* gameObject);

	Texture2D m_sprite;
	int m_width = 0;
	int m_height = 0;
	int m_imageIndex = 0;
	int m_imageSpeed = 0;

	Color m_blend = WHITE;

private:
	float m_imageTimer = 0;
	Rectangle m_drawRect = { 0,0,0,0 };
};