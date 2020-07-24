#include "FollowPathBehavior.h"
#include "Agent.h"

FollowPathBehavior::FollowPathBehavior(const Path& path, const float& speed) : m_path(path), m_speed(speed)
{

}

FollowPathBehavior::~FollowPathBehavior()
{

}

void FollowPathBehavior::Update(Agent* obj, float deltaTime)
{
	if(MoveTowards(obj, m_path.GetPathNodePosition(m_pathPos), deltaTime))
	{
		m_pathPos++;
		if (m_pathPos >= m_path.GetPathSize())
		{
			obj->PopBehaviour();
		}
	}
}

bool FollowPathBehavior::MoveTowards(Agent* obj,Vector2 target,float deltaTime)
{
	Vector2 heading = Vector2Add(obj->GetPosition(), obj->GetPosition());
	Vector2 dirvec = Vector2Subtract(target, obj->GetPosition());

	float dir = atan2(dirvec.y, dirvec.x);

	Vector2 force = { cos(dir),sin(dir) };

	obj->ApplyForce(Vector2Scale(force, m_speed));

	if (Vector2Distance(obj->GetPosition(), target) < ((m_speed * deltaTime)*8))
	{
		return true;
	}
	return false;
}

void FollowPathBehavior::Draw(Agent* obj)
{
	
}
