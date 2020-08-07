#pragma once
#include <vector>
#include <iostream>
#include <map>
#include <typeindex>
#include "Collider.h"
#include "Component.h"
#include "raylib.h"
#include "Drawable.h"

#include "tinyxml2.h"
#include "imgui.h"

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
	virtual ~GameObject() 
	{
		delete m_collider;

		//Comps
		for (auto const com : m_components)
		{
			delete com.second;
		}
	}

	virtual std::type_index GetCategory() { return typeid(GameObject); };
	virtual std::string GetType() { return "GameObject (Shouldn't be saving this one.)"; };

	template<typename T>
	void AddComponent(Component* component)
	{
		m_components[typeid(T)] = component;
	}

	template<typename T>
	T* GetComponent()
	{
		return dynamic_cast<T*>(m_components[typeid(T)]);
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

	virtual void Save(tinyxml2::XMLDocument level, tinyxml2::XMLElement* parentElement) 
	{
		parentElement->SetAttribute("Type", GetType().c_str());
		parentElement->SetAttribute("PositionX", GetPosition().x);
		parentElement->SetAttribute("PositionY", GetPosition().y);
	};
	virtual void Load(tinyxml2::XMLDocument level, tinyxml2::XMLElement* parentElement) 
	{
		parentElement->QueryFloatAttribute("PositionX", &m_position.x);
		parentElement->QueryFloatAttribute("PositionY", &m_position.y);
	};

	virtual void DrawInEditor(const Vector2& mousePos) {};
	
	virtual void ImGuiEditorMenu()
	{
		ImGui::BeginMenu("Entity Edtior");

		ImGui::Text("No custom properties to edit.");

		ImGui::End();
	}

	//SimpleCollision
	bool InRectangle(Rectangle rect)
	{
		return CheckCollisionPointRec(m_position,rect);
	}

	//Getters
	Collider GetCollider() { return *m_collider;};

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
	std::map<std::type_index,Component*> m_components;

	Vector2 m_scale = { 1,1 };
	Vector2 m_origin = {0,0};
	Vector2 m_position = { 0,0 };
	float m_rotation = 0;

	Collider* m_collider = nullptr;
};