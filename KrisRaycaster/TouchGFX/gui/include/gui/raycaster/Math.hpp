#ifndef KRISRAYCASTER_MATH_HPP
#define KRISRAYCASTER_MATH_HPP

struct Vec2f
{
	float x;
	float y;
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
