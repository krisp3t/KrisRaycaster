#ifndef KRISRAYCASTER_MATH_HPP
#define KRISRAYCASTER_MATH_HPP
#include <cmath>

struct Vec2f
{
	float x;
	float y;
	void rotate(float angle)
	{
		float s = sinf(angle);
		float c = cosf(angle);
		float xnew = x * c - y * s;
		float ynew = x * s + y * c;
		x = xnew;
		y = ynew;
	}
};

struct Vec2
{
	int x;
	int y;
};

constexpr Vec2f VEC_FORWARD = {0, -1};
constexpr Vec2f VEC_RIGHT = {1, 0};
constexpr Vec2f VEC_BACKWARD = {0, 1};
constexpr Vec2f VEC_LEFT = {-1, 0};

#endif
