#pragma once

#include <ostream>

#include <math/vector/vec.h>
#include <math/vector/vec3.h>

namespace math
{

template<typename T>
class vec<4, T>
{
public:
    vec<4, T>()
        : x(0), y(0), z(0), w(0)
    {
    }

    vec<4, T>(T v)
        : x(v), y(v), z(v), w(v)
    {
    }

    vec<4, T>(T x, T y, T z, T w)
        : x(x), y(y), z(z), w(w)
    {
    }

    vec<4, T>(T v, T w)
        : x(v), y(v), z(v), w(w)
    {
    }

    vec<4, T>(const vec<3, T>& vec, T w = 1)
        : x(vec.x), y(vec.y), z(vec.z), w(w)
    {
    }

    vec<4, T>(const vec<2, T>& vec, T z = 0, T w = 1)
        : x(vec.x), y(vec.y), z(z), w(w)
    {        
    }

    vec<4, T> operator+(const vec<4, T>& val) const
    {
        return vec<4, T>(x + val.x, y + val.y, z + val.z, w + val.w);
    }

    vec<4, T> operator+(const T& num) const
    {
        return vec<4, T>(x + num, y + num, z + num, w + num);
    }

    vec<4, T> operator-(const vec<4, T>& val) const
    {
        return vec<4, T>(x - val.x, y - val.y, z - val.z, w - val.w);
    }

    vec<4, T> operator-(const T& num) const
    {
        return vec<4, T>(x - num, y - num, z - num, w - num);
    }

    vec<4, T> operator*(const vec<4, T>& val) const
    {
        return vec<4, T>(x * val.x, y * val.y, z * val.z, w * val.w);
    }

    vec<4, T> operator*(const T& scl) const
    {
        return vec<4, T>(x * scl, y * scl, z * scl, w * scl);
    }

    vec<4, T> operator/(const vec<4, T>& val) const
    {
        return vec<4, T>(x / val.x, y / val.y, z / val.z, w / val.w);
    }

    vec<4, T> operator/(const T& scl) const
    {
        return vec<4, T>(x / scl, y / scl, z / scl, w / scl);
    }

    vec<4, T>& operator+=(const vec<4, T>& val)
    {
        *this = *this + val;
        return *this;
    }

    vec<4, T>& operator+=(const T& num)
    {
        *this = *this + num;
        return *this;
    }

    vec<4, T>& operator-=(const vec<4, T>& val)
    {
        *this = *this - val;
        return *this;
    }

    vec<4, T>& operator-=(const T& num)
    {
        *this = *this - num;
        return *this;
    }

    vec<4, T>& operator*=(const vec<4, T>& val)
    {
        *this = *this * val;
        return *this;
    }

    vec<4, T>& operator*=(const T& scl)
    {
        *this = *this * scl;
        return *this;
    }

    vec<4, T>& operator/=(const vec<4, T>& val)
    {
        *this = *this / val;
        return *this;
    }

    vec<4, T>& operator/=(const T& scl)
    {
        *this = *this / scl;
        return *this;
    }

    T& operator[](int index)
    {
        switch (index)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
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
            case 3: return w;
            default: return x;
        }
    }

    std::string str() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
    }

    union { T x, r, s; };
    union { T y, g, t; };
    union { T z, b, p; };
    union { T w, a, q; };
};

typedef vec<4, float>        vec4;
typedef vec<4, int>          ivec4;
typedef vec<4, long>         lvec4;
typedef vec<4, double>       dvec4;
typedef vec<4, unsigned int> uvec4;

}