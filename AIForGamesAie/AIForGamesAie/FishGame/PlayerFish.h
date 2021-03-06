#pragma once
#include "Agent.h"
#include "Clam.h"

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
	virtual void Hurt();

private:
	Drawable* m_drawable;
	float m_hitTimer = 0;
	int m_playerHealth = 3;
	bool m_dead = false;
	bool m_recentlyHit = false;

	Clam* m_nearestClam = nullptr;
	float m_nearestClamRotation = 0;
};

