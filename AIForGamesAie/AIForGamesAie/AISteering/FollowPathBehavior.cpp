#include "FollowPathBehavior.h"
#include "Agent.h"
#include "Path.h"

FollowPathBehavior::FollowPathBehavior(Path* path, const float& speed) :  m_speed(speed)
{
	m_path = path;
}

FollowPathBehavior::~FollowPathBehavior()
{
	delete m_path;
}

void FollowPathBehavior::Update(Agent* obj, float deltaTime)
{
	if(MoveTowards(obj, m_path->GetPathNodePosition(m_pathPos), deltaTime))
	{
		m_pathPos += m_direction;

		if (m_pathPos >= m_path->GetPathSize() || m_pathPos < 0)
		{
			//DoPath
			switch (m_path->GetPathType())
			{
				case PathType::Open:
				{
					obj->PopBehaviour();
					break;
				}

				case PathType::Closed:
				{
					m_pathPos = 0;
					break;
				}

				case PathType::Reverse:
				{
					m_direction = -m_direction;
					m_pathPos += m_direction;
					break;
				}
			}
			
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

	if (Vector2Distance(obj->GetPosition(), target) < (obj->GetCollider().BBoxWidth()))
	{
		return true;
	}
	return false;
}

void FollowPathBehavior::Draw(Agent* obj)
{
	
}
