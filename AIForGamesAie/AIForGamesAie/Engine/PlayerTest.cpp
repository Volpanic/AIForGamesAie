#include "PlayerTest.h"

PlayerTest::PlayerTest()
{
	sprite = LoadTexture("Sprites/spr_save.png");
	Drawable* dr = new Drawable(sprite, 16, 16);
	drawable = (ComponentPtr)(dr);
	m_position.x += 16;
	AddComponent(drawable);
}

PlayerTest::~PlayerTest()
{
	UnloadTexture(sprite);
}

void PlayerTest::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}

void PlayerTest::Draw()
{
	GameObject::Draw();
}

void MoveX();