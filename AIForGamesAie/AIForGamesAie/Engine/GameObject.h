#pragma once
#include <vector>
#include <iostream>
#include <map>
#include <typeindex>
#include "Component.h"
#include "raylib.h"
#include "Drawable.h"

struct Transform2D
{
	Vector2 m_position;
	Vector2 m_scale;
	Vector2 m_origin;
	Vector2 m_rotation;
};

class GameObject
{
public:
	GameObject() {};
	virtual ~GameObject() {};

	template<typename T>
	void AddComponent(const T& component)
	{
		m_components[typeid(T)] = (ComponentPtr)component;
	}

	template<typename T>
	T* GetComponent()
	{
		return (T*)(m_components[typeid(T)].get());
	}

	virtual void Update(float deltaTime)
	{
		if (!m_active) { return; }
		for (auto& component : m_components)
		{
			component.second->Update(this,deltaTime);
		}
	}

	virtual void Draw()
	{
		if (!m_visible) { return;}
		for (auto& component : m_components)
		{
			component.second->Draw(this);
		}
	}

	//Getters
	Rectangle GetBBox() { return {m_position.x + m_origin.x,m_position.y + m_origin.y,m_boundingBox.width,m_boundingBox.height}; };
	float InRectangle(Rectangle otherRec) { return CheckCollisionRecs(m_boundingBox, otherRec); }

	Vector2& GetPosition() { return m_position;};
	Vector2& GetScale() { return m_scale; };
	Vector2& GetOrigin() { return m_origin; };
	float& GetRotation() { return m_rotation; };

	void SetOrigin(float x, float y) { m_origin.x = x; m_origin.y = y; };
	void SetPosition(float x, float y) { m_position.x = x; m_position.y = y; };
	void SetScale(float x, float y) { m_scale.x = x; m_scale.y; };
	void SetRotation(float rotation) { m_rotation = rotation;};

	//Variables
	bool m_visible = true;
	bool m_active = true;

protected:
	std::map<std::type_index,ComponentPtr> m_components;
	Rectangle m_boundingBox = {0,0,1,1};

	Vector2 m_scale = { 1,1 };
	Vector2 m_origin = {0,0};
	Vector2 m_position = { 0,0 };
	float m_rotation = 0;
};