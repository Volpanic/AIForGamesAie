#include "DarkBlueShark.h"

DarkBlueShark::DarkBlueShark(LevelState* level) : Agent::Agent(level)
{
	SetOrigin(11, 6);
	m_collider = new Collider();
	m_collider->Setup(this, 24, 10);

	AddComponent<Drawable>(new Drawable(m_level->GetResources()->GetTexture("spr_dark_blue_shark.png"), 27, 13));
}

DarkBlueShark::~DarkBlueShark()
{

}

void DarkBlueShark::Update(float deltaTime)
{
	Agent::Update(deltaTime);
}

void DarkBlueShark::Draw()
{
	Agent::Draw();
}

void DarkBlueShark::DrawInEditor(const Vector2& mousePos)
{
	m_position = mousePos;
	Agent::Draw();
	DrawRectangleLinesEx(m_collider->GetBBox(), 1, GREEN);
}
