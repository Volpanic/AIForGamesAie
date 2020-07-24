#pragma once
#include "Behaviour.h"
#include "Path.h"

class FollowPathBehavior : public Behaviour
{
public:
	FollowPathBehavior(const Path& path, const float& speed);
	virtual ~FollowPathBehavior();

	bool MoveTowards(Agent* obj,Vector2 target, float deltaTime);
	virtual void Update(Agent* obj, float deltaTime);
	virtual void Draw(Agent* obj);

protected:
	float m_speed;
	int m_pathPos = 0;
	Path m_path;
};

