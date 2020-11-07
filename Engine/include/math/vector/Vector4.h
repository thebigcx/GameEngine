#pragma once

#include <ostream>

#include <math/vector/Vector.h>
#include <math/vector/Vector3.h>

template<typename T>
class Vector<4, T>
{
public:
    Vector<4, T>()
        : x(0), y(0), z(0), w(0)
    {
    }

    Vector<4, T>(T x, T y, T z, T w)
        : x(x), y(y), z(z), w(w)
    {
    }

    Vector<4, T>(T v, T w)
        : x(v), y(v), z(v), w(w)
    {
    }

    Vector<4, T>(const Vector<3, T>& vec, T w)
        : x(vec.x), y(vec.y), z(vec.z), w(w)
    {
    }

    Vector<4, T> operator+(const Vector<4, T> vec) const
    {
        return Vector<4, T>(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
    }

    Vector<4, T> operator+(const T& num) const
    {
        return Vector<4, T>(x + num, y + num, z + num, w + num);
    }

    Vector<4, T> operator-(const Vector<4, T> vec) const
    {
        return Vector<4, T>(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
    }

    Vector<4, T> operator-(const T& num) const
    {
        return Vector<4, T>(x - num, y - num, z - num, w - num);
    }

    Vector<4, T> operator*(const Vector<4, T> vec) const
    {
        return Vector<4, T>(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
    }

    Vector<4, T> operator*(const T& scl) const
    {
        return Vector<4, T>(x * scl, y * scl, z * scl, w * scl);
    }

    Vector<4, T> operator/(const Vector<4, T> vec) const
    {
        return Vector<4, T>(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
    }

    Vector<4, T> operator/(const T& scl) const
    {
        return Vector<4, T>(x / scl, y / scl, z / scl, w / scl);
    }

    Vector<4, T>& operator+=(const Vector<4, T> vec)
    {
        *this = *this + vec;
        return *this;
    }

    Vector<4, T>& operator+=(const T& num)
    {
        *this = *this + num;
        return *this;
    }

    Vector<4, T>& operator-=(const Vector<4, T> vec)
    {
        *this = *this - vec;
        return *this;
    }

    Vector<4, T>& operator-=(const T& num)
    {
        *this = *this - num;
        return *this;
    }

    Vector<4, T>& operator*=(const Vector<4, T> vec)
    {
        *this = *this * vec;
        return *this;
    }

    Vector<4, T>& operator*=(const T& scl)
    {
        *this = *this * scl;
        return *this;
    }

    Vector<4, T>& operator/=(const Vector<4, T> vec)
    {
        *this = *this / vec;
        return *this;
    }

    Vector<4, T>& operator/=(const T& scl)
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
            default: return (T)0;
        }
    }

    std::string str() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
    }

    T x, y, z, w;
};

typedef Vector<4, float>        Vector4f;
typedef Vector<4, int>          Vector4i;
typedef Vector<4, long>         Vector4l;
typedef Vector<4, double>       Vector4d;
typedef Vector<4, unsigned int> Vector4u;