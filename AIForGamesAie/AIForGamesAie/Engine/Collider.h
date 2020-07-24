#pragma once
#include "raylib.h"
#include <list>

class GameObject;

class Collider
{
public:
	Collider();
	void Setup(GameObject* parent, int width, int height);
	Rectangle GetBBox();

	float BBoxLeft();
	float BBoxRight();
	float BBoxTop();
	float BBoxBottom();
	float BBoxWidth();
	float BBoxHeight();

	Vector2 BBoxTopLeft() { return {BBoxLeft(),BBoxTop()};};
	Vector2 BBoxTopRight() { return {BBoxRight(),BBoxTop()}; };
	Vector2 BBoxBottomLeft() { return { BBoxLeft(),BBoxBottom()}; };
	Vector2 BBoxBottomRight() { return { BBoxRight(),BBoxBottom()}; };

	Vector2 BBoxCenter();

	bool CollideAt(Rectangle rect, const Vector2& position);
	bool CollideAt(std::list<Rectangle> rects, const Vector2& position);
	bool CollideAt(Collider coll, const Vector2& position);
	bool CollideAt(std::list<Collider> colls, const Vector2& position);

private:
	GameObject* m_parent = nullptr;
	int m_boxWidth = 1;
	int m_boxHeight = 1;
};

