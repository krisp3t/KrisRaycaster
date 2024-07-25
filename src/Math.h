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

    // TODO: add normalized
};

template<>
struct Vec<2, int>
{
    int x, y;

    Vec<2, int> Rotate(float angle) const
    {
        Vec<2, int> result{};
        result.x = this->x * cos(angle) - this->y * sin(angle);
        result.y = this->x * sin(angle) + this->y * cos(angle);
        return result;
    }

    Vec<2, int> operator+(const Vec<2, int> &other) const
    {
        Vec<2, int> result{};
        result.x = this->x + other.x;
        result.y = this->y + other.y;
        return result;
    }

    Vec<2, int> operator-(const Vec<2, int> &other) const
    {
        Vec<2, int> result{};
        result.x = this->x - other.x;
        result.y = this->y - other.y;
        return result;
    }

    Vec<2, int> operator+=(const Vec<2, int> &other) const
    {
        Vec<2, int> result{};
        result.x = this->x + other.x;
        result.y = this->y + other.y;
        return result;
    }

    Vec<2, int> operator*(int scalar) const
    {
        Vec<2, int> result{};
        result.x = this->x * scalar;
        result.y = this->y * scalar;
        return result;
    }
};

template<>
struct Vec<2, float>
{
    float x, y;

    Vec<2, float> Rotate(float angle) const
    {
        Vec<2, float> result{};
        result.x = this->x * cos(-angle) - this->y * sin(-angle);
        result.y = this->x * sin(-angle) + this->y * cos(-angle);
        return result;
    }

    Vec<2, float> operator+(const Vec<2, float> &other) const
    {
        Vec<2, float> result{};
        result.x = this->x + other.x;
        result.y = this->y + other.y;
        return result;
    }

    Vec<2, float> operator+(float other) const
    {
        Vec<2, float> result{};
        result.x = this->x + other;
        result.y = this->y + other;
        return result;
    }
    
    Vec<2, float> operator-(const Vec<2, float> &other) const
    {
        Vec<2, float> result{};
        result.x = this->x - other.x;
        result.y = this->y - other.y;
        return result;
    }

    Vec<2, float> operator*(float scalar) const
    {
        Vec<2, float> result{};
        result.x = this->x * scalar;
        result.y = this->y * scalar;
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

