#pragma once
#include <algorithm>

static class Numbers
{
public:
	template <typename T>
	static int Sign(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	static float Approach(float value, float target, float amount)
	{
		if (value < target)
		{
			value = std::min(value + amount, target);
		}
		else
		{
			value = std::max(value - amount, target);
		}
		return value;
	}

	static int Approach(int value, int target, int amount)
	{
		if (value < target)
		{
			value = std::min(value + amount, target);
		}
		else
		{
			value = std::max(value - amount, target);
		}
		return value;
	}

	static int FloorMultiple(int value, int multiple)
	{
		return (int)floor((value / multiple))*multiple;
	}

	static float FloorMultiple(float value, int multiple)
	{
		return floor((value / multiple)) * multiple;
	}

	static Vector2 FloorMultiple(Vector2 value, int multiple)
	{
		return {FloorMultiple(value.x,multiple),FloorMultiple(value.y,multiple)};
	}
};