#include "Collider.h"
#include "GameObject.h"

Collider::Collider()
{
	m_boundingBox = { 0,0,1,1};
}

void Collider::Setup(GameObject* parent, int width, int height)
{
	m_parent = parent;
	m_boundingBox.width = width;
	m_boundingBox.height = height;
}

Rectangle Collider::GetBBox()
{
	return {m_parent->GetPosition().x - m_parent->GetOrigin().x, m_parent->GetPosition().y - m_parent->GetOrigin().y,m_boundingBox.width,m_boundingBox.height};
}


float Collider::BBoxLeft()
{
	return GetBBox().x;
}

float Collider::BBoxRight()
{
	return GetBBox().x + m_boundingBox.width;
}

float Collider::BBoxTop()
{
	return GetBBox().y;
}

float Collider::BBoxBottom()
{
	return GetBBox().y + m_boundingBox.height;
}

float Collider::BBoxWidth()
{
	return m_boundingBox.width;
}

float Collider::BBoxHeight()
{
	return m_boundingBox.height;
}

Vector2 Collider::BBoxCenter()
{
	return { GetBBox().x + (m_boundingBox.width/2),GetBBox().y + (m_boundingBox.height / 2) };
}

bool Collider::CollideAt(Rectangle rect, const Vector2& position)
{
	Rectangle box = GetBBox();
	box.x = position.x;
	box.y = position.y;

	return CheckCollisionRecs(rect, box);
}

bool Collider::CollideAt(std::list<Rectangle> rects, const Vector2& position)
{

	if (rects.empty()) return false;

	for (auto const& rect : rects)
	{
		if (CollideAt(rect, position))
		{
			return true;
		}
	}
	return false;
}

bool Collider::CollideAt(Collider coll, const Vector2& position)
{
	return CollideAt(coll.GetBBox(),position);
}

bool Collider::CollideAt(std::list<Collider> colls, const Vector2& position)
{
	if (colls.empty()) return false;

	for (auto const& coll : colls)
	{
		if (CollideAt(coll, position))
		{
			return true;
		}
	}
	return false;
}
