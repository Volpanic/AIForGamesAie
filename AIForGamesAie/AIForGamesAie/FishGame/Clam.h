#pragma once
#include "GameObject.h"

class Clam : public GameObject
{
public:
	Clam(LevelState* level);
	virtual ~Clam();

	virtual std::type_index GetCategory() { return typeid(Clam); };
	virtual std::string GetType() { return "Clam"; };

	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void DrawInEditor(const Vector2& mousePos);

	bool HasPearl() { return m_hasPearl; };
private:
	LevelState* m_level = nullptr;
	Drawable* m_drawable = nullptr;
	bool m_hasPearl = true;
};

