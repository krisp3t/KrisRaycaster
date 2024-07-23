#pragma once

template<int n, typename T>
struct Vec
{
    T v[n];

    T &operator[](int i)
    { return v[i]; }

    const float &operator[](int i) const
    { return v[i]; }
    // TODO: add normalized

};

template<>
struct Vec<2, int>
{
    int x, y;
};
template<>
struct Vec<2, float>
{
    float x, y;
};
template<>
struct Vec<3, int>
{
    int x, y, z;
};
template<>
struct Vec<3, float>
{
    float x, y, z;
};

typedef Vec<2, int> Vec2;
typedef Vec<2, float> Vec2f;
typedef Vec<3, int> Vec3;
typedef Vec<3, float> Vec3f;