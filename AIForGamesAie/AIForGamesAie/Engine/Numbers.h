#pragma once

static class Numbers
{
public:
	template <typename T>
	static int Sign(T val)
	{
		return (T(0) < val) - (val < T(0));
	}
};