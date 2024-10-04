#pragma once

#include <algorithm>
#include <cmath>
#include <array>
#include <iostream>

template<typename T, std::size_t N>
class Vec;

template<typename T>
class Vec<T, 2>
{
public:
    union
    {
        std::array<T, 2> e;
        struct
        {
            T x, y;
        };
        struct
        {
            T r, g;
        };
    };

    // Default ctor - Vector.Zero
    Vec() : e{}
    {}

    // Ctor with specified values
    Vec(T x, T y) : e{x, y}
    {}

    T operator[](std::size_t i) const
    { return e[i]; }

    T &operator[](std::size_t i)
    { return e[i]; }

    [[nodiscard]] T LengthSquared() const
    {
        return x * x + y * y;
    }

    [[nodiscard]] T Length() const
    {
        return abs(x * x + y * y);
    }

    Vec &operator+=(const Vec &v)
    {
        for (std::size_t i = 0; i < 2; ++i)
        {
            e[i] += v.e[i];
        }
        return *this;
    }

    Vec &operator+=(T t)
    {
        for (std::size_t i = 0; i < 2; ++i)
        {
            e[i] += t;
        }
        return *this;
    }

    Vec &operator-=(const Vec &v)
    {
        for (std::size_t i = 0; i < 2; ++i)
        {
            e[i] -= v.e[i];
        }
        return *this;
    }

    Vec &operator-=(T t)
    {
        for (std::size_t i = 0; i < 2; ++i)
        {
            e[i] -= t;
        }
        return *this;
    }

    Vec &operator*=(const Vec &v)
    {
        for (std::size_t i = 0; i < 2; ++i)
        {
            e[i] *= v.e[i];
        }
        return *this;
    }

    Vec &operator*=(T t)
    {
        for (std::size_t i = 0; i < 2; ++i)
        {
            e[i] *= t;
        }
        return *this;
    }

    Vec &operator/=(T t)
    {
        for (std::size_t i = 0; i < 2; ++i)
        {
            e[i] /= t;
        }
        return *this;
    }

    void Rotate(float angle)
    {
        float s = std::sin(angle);
        float c = std::cos(angle);
        float newX = x * c - y * s;
        float newY = x * s + y * c;
        x = newX;
        y = newY;
    }
};

template<typename T>
class Vec<T, 3>
{
public:
    union
    {
        std::array<T, 3> e;
        struct
        {
            T x, y, z;
        };
        struct
        {
            T r, g, b;
        };
    };

    // Default ctor - Vector.Zero
    Vec() : e{}
    {}

    // Ctor with specified values
    Vec(T x, T y, T z) : e{x, y, z}
    {}

    T operator[](std::size_t i) const
    { return e[i]; }

    T &operator[](std::size_t i)
    { return e[i]; }

    [[nodiscard]] T LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    [[nodiscard]] T Length() const
    {
        return abs(x * x + y * y + z * z);
    }

    Vec &operator+=(const Vec &v)
    {
        for (std::size_t i = 0; i < 3; ++i)
        {
            e[i] += v.e[i];
        }
        return *this;
    }

    Vec &operator+=(T t)
    {
        for (std::size_t i = 0; i < 3; ++i)
        {
            e[i] += t;
        }
        return *this;
    }

    Vec &operator-=(const Vec &v)
    {
        for (std::size_t i = 0; i < 3; ++i)
        {
            e[i] -= v.e[i];
        }
        return *this;
    }

    Vec &operator-=(T t)
    {
        for (std::size_t i = 0; i < 3; ++i)
        {
            e[i] -= t;
        }
        return *this;
    }

    Vec &operator*=(const Vec &v)
    {
        for (std::size_t i = 0; i < 3; ++i)
        {
            e[i] *= v.e[i];
        }
        return *this;
    }

    Vec &operator*=(T t)
    {
        for (std::size_t i = 0; i < 3; ++i)
        {
            e[i] *= t;
        }
        return *this;
    }

    Vec &operator/=(T t)
    {
        for (std::size_t i = 0; i < 3; ++i)
        {
            e[i] /= t;
        }
        return *this;
    }
};

using Vec2f = Vec<float, 2>;
using Vec3f = Vec<float, 3>;
using Vec2 = Vec<int, 2>;
using Vec3 = Vec<int, 3>;

// Vec operators
template<typename T, std::size_t N>
inline Vec<T, N> operator+(const Vec<T, N> &v1, const Vec<T, N> &v2)
{
    Vec<T, N> result = v1;
    result += v2;
    return result;
}

template<typename T, std::size_t N>
inline Vec<T, N> operator+(const Vec<T, N> &v, T t)
{
    Vec<T, N> result = v;
    result += t;
    return result;
}

template<typename T, std::size_t N>
inline Vec<T, N> operator-(const Vec<T, N> &v1, const Vec<T, N> &v2)
{
    Vec<T, N> result = v1;
    result -= v2;
    return result;
}

template<typename T, std::size_t N>
inline Vec<T, N> operator-(const Vec<T, N> &v, T t)
{
    Vec<T, N> result = v;
    result -= t;
    return result;
}

template<typename T, std::size_t N>
inline Vec<T, N> operator*(const Vec<T, N> &v1, const Vec<T, N> &v2)
{
    Vec<T, N> result = v1;
    result *= v2;
    return result;
}

template<typename T, std::size_t N>
inline Vec<T, N> operator*(const Vec<T, N> &v, T t)
{
    Vec<T, N> result = v;
    result *= t;
    return result;
}

template<typename T, std::size_t N>
inline Vec<T, N> operator*(T t, const Vec<T, N> &v)
{
    Vec<T, N> result = v;
    result *= t;
    return result;
}

template<typename T, std::size_t N>
inline T Dot(const Vec<T, N> &v1, const Vec<T, N> &v2)
{
    T result = 0;
    for (std::size_t i = 0; i < N; ++i)
    {
        result += v1.e[i] * v2.e[i];
    }
    return result;
}

inline Vec3f Cross(const Vec3f &v1, const Vec3f &v2)
{
    return Vec3f{v1.y * v2.z - v1.z * v2.y,
                 v1.z * v2.x - v1.x * v2.z,
                 v1.x * v2.y - v1.y * v2.x};
}

// Print Vec
template<typename T, std::size_t N>
inline std::ostream &operator<<(std::ostream &os, const Vec<T, N> &v)
{
    os << '(';
    for (std::size_t i = 0; i < N; ++i)
    {
        os << v.e[i];
        if (i < N - 1)
        {
            os << ", ";
        }
    }
    os << ')';
}

inline float DegToRad(float deg)
{
    return deg * M_PI / 180;
}

inline float RadToDeg(float rad)
{
    return rad * 180 / M_PI;
}

inline Vec2 MapToScreen(Vec2f pos, Vec2 screenSize, int mapSize)
{
    Vec2 result{};
    result.x = floor(pos.x * screenSize.x / mapSize);
    result.y = floor(pos.y * screenSize.y / mapSize);
    return result;
}


inline uint32_t ApplyBrightnessAbgr(uint32_t color, int brightnessScale)
{
    uint8_t a = (color >> 24) & 0xFF;
    uint8_t b = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t r = color & 0xFF; // Alpha remains unchanged

    r = (r * brightnessScale) >> 8;
    g = (g * brightnessScale) >> 8;
    b = (b * brightnessScale) >> 8;

    r = r > 255 ? 255 : r;
    g = g > 255 ? 255 : g;
    b = b > 255 ? 255 : b;
    return (a << 24) | (b << 16) | (g << 8) | r;
}
