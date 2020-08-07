#pragma once
#include "Agent.h"

class PlayerFish : public Agent
{
public:
	PlayerFish(LevelState* level);
	virtual ~PlayerFish();

	virtual std::type_index GetCategory() { return typeid(PlayerFish); };
	virtual std::string GetType() { return "PlayerFish"; };

	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void DrawInEditor(const Vector2& mousePos);

private:
	Drawable* m_drawable;
};

