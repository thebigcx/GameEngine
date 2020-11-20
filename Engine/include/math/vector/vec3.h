#pragma once

#include <ostream>
#include <cmath>

#include <math/vector/vec.h>
#include <math/vector/vec2.h>

namespace math
{

template<typename T>
class vec<3, T>
{
public:
    vec<3, T>()
        : x(0), y(0), z(0)
    {
    }

    vec<3, T>(T x, T y, T z)
        : x(x), y(y), z(z)
    {
    }

    vec<3, T>(T v)
        : x(v), y(v), z(v)
    {
    }

    vec<3, T>(const vec<2, T>& vec, T z)
        : x(vec.x), y(vec.y), z(z)
    {
    }

    void normalize()
    {
        *this = vec<3, T>::normalize(*this);
    }

    vec<3, T> cross(const vec<3, T>& other)
    {
        return vec<3, T>::cross(*this, other);
        
    }

    float mag() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    static vec<3, T> normalize(const vec<3, T>& val)
    {
        float magnitude = 1.f / val.mag();
        return vec<3, T>(val.x * magnitude, val.y * magnitude, val.z * magnitude);
    }

    static vec<3, T> cross(const vec<3, T>& first, const vec<3, T>& second)
    {
        return vec<3, T>(first.y * second.z - first.z * second.y, first.z * second.x - first.x * second.z, first.x * second.y - first.y * second.x);
    }

    static float dot(const vec<3, T>& first, const vec<3, T>& second)
    {
        return first.x * second.x + first.y * second.y + first.z * second.z;
    }

    vec<3, T> operator+(const vec<3, T>& val) const
    {
        return vec<3, T>(x + val.x, y + val.y, z + val.z);
    }

    vec<3, T> operator+(const T& num) const
    {
        return vec<3, T>(x + num, y + num, z + num);
    }

    vec<3, T> operator-(const vec<3, T>& val) const
    {
        return vec<3, T>(x - val.x, y - val.y, z - val.z);
    }

    vec<3, T> operator-(const T& num) const
    {
        return vec<3, T>(x - num, y - num, z - num);
    }

    vec<3, T> operator*(const vec<3, T>& val) const
    {
        return vec<3, T>(x * val.x, y * val.y, z * val.z);
    }

    vec<3, T> operator*(const T& scl) const
    {
        return vec<3, T>(x * scl, y * scl, z * scl);
    }

    vec<3, T> operator/(const vec<3, T>& val) const
    {
        return vec<3, T>(x / val.x, y / val.y, z / val.z);
    }

    vec<3, T> operator/(const T& scl) const
    {
        return vec<3, T>(x / scl, y / scl, z / scl);
    }

    vec<3, T>& operator+=(const vec<3, T>& val)
    {
        *this = *this + val;
        return *this;
    }

    vec<3, T>& operator+=(const T& num)
    {
        x += num;
        y += num;
        z += num;
        return *this;
    }

    vec<3, T>& operator-=(const vec<3, T>& val)
    {
        *this = *this - val;
        return *this;
    }

    vec<3, T>& operator-=(const T& num)
    {
        x -= num;
        y -= num;
        z -= num;
        return *this;
    }

    vec<3, T>& operator*=(const vec<3, T>& val)
    {
        *this = *this * val;
        return *this;
    }

    vec<3, T>& operator*=(const T& scl)
    {
        x *= scl;
        y *= scl;
        z *= scl;
        return *this;
    }

    vec<3, T>& operator/=(const vec<3, T>& val)
    {
        *this = *this / val;
        return *this;
    }

    vec<3, T>& operator/=(const T& scl)
    {
        x /= scl;
        y /= scl;
        z /= scl;
        return *this;
    }

    bool operator==(const vec<3, T>& val)
    {
        return x == val.x && y == val.y && z == val.z;
    }

    bool operator!=(const vec<3, T>& val)
    {
        return !operator==(val);
    }

    T& operator[](int index)
    {
        switch (index)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: return x;
        }
    }

    T operator[](int index) const
    {
        switch (index)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: return x;
        }
    }

    std::string str() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }

    union { T x, r, s; };
    union { T y, g, t; };
    union { T z, b, p; };
};

typedef vec<3, float>        vec3;
typedef vec<3, int>          ivec3;
typedef vec<3, long>         lvec3;
typedef vec<3, double>       dvec3;
typedef vec<3, unsigned int> uvec3;

}