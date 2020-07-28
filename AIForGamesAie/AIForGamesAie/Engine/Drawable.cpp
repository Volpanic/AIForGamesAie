#include <iostream>
#include "Drawable.h"
#include "raylib.h"

Drawable::Drawable(Texture2D sprite, int width, int height)
{
	m_sprite = sprite;
	m_width = width;
	m_height = height;

	m_drawRect.width = m_width;
	m_drawRect.height = m_height;
}

void Drawable::SetDrawRect(int frame)
{
	int xPos = 0;
	int yPos = 0;
	while (frame != 0)
	{
		xPos += m_width;

		if (xPos > m_sprite.width)
		{
			yPos += m_height;
			xPos = 0;

			if (yPos > m_sprite.height)
			{
				yPos = 0;
			}
		}
		frame--;
	}

	m_drawRect.x = xPos;
	m_drawRect.y = yPos;
}

void Drawable::Update(GameObject* gameObject, float deltaTime)
{
	if (m_imageSpeed != 0)
	{
		m_imageTimer += 1;

		if (m_imageTimer >= m_imageSpeed)
		{
			m_imageIndex++;
			SetDrawRect(m_imageIndex);
			m_imageTimer -= m_imageSpeed;
		}
	}
}

void Drawable::Draw(GameObject* gameObject)
{
	DrawTexturePro(m_sprite, m_drawRect, { gameObject->GetPosition().x,gameObject->GetPosition().y,(float)m_width,(float)m_height }, gameObject->GetOrigin(), gameObject->GetRotation() * RAD2DEG, WHITE);
}