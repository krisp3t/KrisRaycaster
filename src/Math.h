#pragma once

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

template<int n, typename T>
struct Vec
{
    T v[n];

    T &operator[](int i)
    { return v[i]; }

    const float &operator[](int i) const
    { return v[i]; }

    Vec<n, T> operator+(const Vec<n, T> &other) const
    {
        Vec<n, T> result;
        for (int i = 0; i < n; i++)
            result[i] = v[i] + other[i];
        return result;
    }
    // TODO: add normalized
};

template<>
struct Vec<2, int>
{
    int x, y;

    Vec<2, int> operator*(float angle) const
    {
        Vec<2, int> result{};
        result.x = this->x * cos(angle) - this->y * sin(angle);
        result.y = this->x * sin(angle) + this->y * cos(angle);
        return result;
    }
};

template<>
struct Vec<2, float>
{
    float x, y;

    Vec<2, float> operator*(float angle) const
    {
        Vec<2, float> result{};
        result.x = this->x * cos(-angle) - this->y * sin(-angle);
        result.y = this->x * sin(-angle) + this->y * cos(-angle);
        return result;
    }
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

inline float DegToRad(float deg)
{
    return deg * M_PI / 180;
}

inline float RadToDeg(float rad)
{
    return rad * 180 / M_PI;
}

inline float Fmax(float a, float b)
{
    return a > b ? a : b;
}

