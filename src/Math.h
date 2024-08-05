#pragma once

#include <cmath>
#include <array>
#include <iostream>


template<typename T, std::size_t N>
class Vec
{
public:
    union
    {
        std::array<T, N> e;
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
    Vec(T x, T y) : e{x, y}
    {
        static_assert(N == 2, "Vec2 ctor with 2 values");
    }

    Vec(T x, T y, T z) : e{x, y, z}
    {
        static_assert(N == 3, "Vec3 ctor with 3 values");
    }

    T operator[](std::size_t i) const
    { return e[i]; }

    T &operator[](std::size_t i)
    { return e[i]; }

    [[nodiscard]] T LengthSquared() const
    {
        T sum = 0;
        for (std::size_t i = 0; i < N; ++i)
        {
            sum += e[i] * e[i];
        }
        return sum;
    }

    [[nodiscard]] T Length() const
    {
        return std::sqrt(LengthSquared());
    }

    Vec &operator+=(const Vec &v)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            e[i] += v.e[i];
        }
        return *this;
    }

    Vec &operator+=(T t)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            e[i] += t;
        }
        return *this;
    }

    Vec &operator-=(const Vec &v)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            e[i] -= v.e[i];
        }
        return *this;
    }

    Vec &operator-=(T t)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            e[i] -= t;
        }
        return *this;
    }

    Vec &operator*=(const Vec &v)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            e[i] *= v.e[i];
        }
        return *this;
    }

    Vec &operator*=(T t)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            e[i] *= t;
        }
        return *this;
    }

    Vec &operator/=(T t)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            e[i] /= t;
        }
        return *this;
    }

    template<typename U = T>
    typename std::enable_if<N == 2 && std::is_same<U, float>::value, void>::type
    Rotate(float angle)
    {
        float s = std::sin(angle);
        float c = std::cos(angle);
        float newX = x * c - y * s;
        float newY = x * s + y * c;
        x = newX;
        y = newY;
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
    result += v2;
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
