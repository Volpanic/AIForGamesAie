#pragma once
#include <iostream>

class GameObject;

class Component
{
public:
	virtual ~Component() { };
	virtual void Update(GameObject* gameObject,float deltaTime) = 0;
	virtual void Draw(GameObject* gameObject) = 0;
};

typedef std::shared_ptr<Component> ComponentPtr;