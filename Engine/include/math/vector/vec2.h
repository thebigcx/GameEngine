#pragma once

#include <ostream>

#include <math/vector/vec.h>

namespace math
{

template<typename T>
class vec<2, T>
{
public:
    vec<2, T>()
        : x(0), y(0)
    {
    }

    vec<2, T>(T x, T y)
        : x(x), y(y)
    {
    }

    vec<2, T>(T v)
        : x(v), y(v)
    {
    }

    vec<2, T> operator+(const vec<2, T>& val) const
    {
        return vec<2, T>(x + val.x, y + val.y);
    }

    vec<2, T> operator+(const T& num) const
    {
        return vec<2, T>(x + num, y + num);
    }

    vec<2, T> operator-(const vec<2, T>& val) const
    {
        return vec<2, T>(x - val.x, y - val.y);
    }

    vec<2, T> operator-(const T& num) const
    {
        return vec<2, T>(x - num, y - num);
    }

    vec<2, T> operator*(const vec<2, T>& val) const
    {
        return vec<2, T>(x * val.x, y * val.y);
    }

    vec<2, T> operator*(const T& scl) const
    {
        return vec<2, T>(x * scl, y * scl);
    }

    vec<2, T> operator/(const vec<2, T>& val) const
    {
        return vec<2, T>(x / val.x, y / val.y);
    }

    vec<2, T> operator/(const T& scl) const
    {
        return vec<2, T>(x / scl, y / scl);
    }

    vec<2, T>& operator+=(const vec<2, T>& val)
    {
        *this = *this + val;
        return *this;
    }

    vec<2, T>& operator+=(const T& num)
    {
        x = x + num;
        y = y + num;
        return *this;
    }

    vec<2, T>& operator-=(const vec<2, T>& val)
    {
        *this = *this - val;
        return *this;
    }

    vec<2, T>& operator-=(const T& num)
    {
        x = x - num;
        y = y - num;
        return *this;
    }

    vec<2, T>& operator*=(const vec<2, T>& val)
    {
        *this = *this * val;
        return *this;
    }

    vec<2, T>& operator*=(const T& scl)
    {
        x = x * scl;
        y = y * scl;
        return *this;
    }

    vec<2, T>& operator/=(const vec<2, T>& val)
    {
        *this = *this / val;
        return *this;
    }

    vec<2, T>& operator/=(const T& scl)
    {
        x = x / scl;
        y = y / scl;
        return *this;
    }
    
    bool operator==(const vec<2, T>& val)
    {
        return x == val.x && y == val.y;
    }

    bool operator!=(const vec<2, T>& val)
    {
        return !operator==(val);
    }

    float mag()
    {
        return sqrt(x * x, y * y);
    }

    void normalize()
    {
        vec<2, T>::normalize(*this);
    }

    float dot(const vec<2, T>& val)
    {
        return vec<2, T>::dot(*this, val);
    }

    static vec<2, T> normalize(const vec<2, T>& val)
    {
        float magnitude = 1.f / mag(); // Divide once (save CPU cycles)
        return vec<2, T>(val.x * magnitude, val.y * magnitude);
    }

    static float dot(const vec<2, T>& vec1, const vec<2, T>& vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    std::string str() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    union { T x, r, s; };
    union { T y, g, t; };
};

typedef vec<2, float>        vec2;
typedef vec<2, int>          ivec2;
typedef vec<2, long>         lvec2;
typedef vec<2, double>       dvec2;
typedef vec<2, unsigned int> uvec2;

}