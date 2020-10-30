#pragma once

#include <ostream>

#include <util/math/vector/Vector2.h>

template<int length, typename T>
class Vector;

template <typename T>
class Vector<3, T>
{
public:
    Vector<3, T>()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    Vector<3, T>(T x, T y, T z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector<3, T>(T v)
    {
        x = v;
        y = v;
        z = v;
    }

    Vector<3, T>(Vector<2, T> vec, T z)
    {
        this->x = vec.x;
        this->y = vec.y;
        this->z = z;
    }

    Vector<3, T> operator+(const Vector<3, T> vec) const
    {
        return Vector<3, T>(x + vec.x, y + vec.y, z + vec.z);
    }

    Vector<3, T> operator+(const T& num) const
    {
        return Vector<3, T>(x + num, y + num, z + num);
    }

    Vector<3, T> operator-(const Vector<3, T> vec) const
    {
        return Vector<3, T>(x - vec.x, y - vec.y, z - vec.z);
    }

    Vector<3, T> operator-(const T& num) const
    {
        return Vector<3, T>(x - num, y - num, z - num);
    }

    Vector<3, T> operator*(const Vector<3, T> vec) const
    {
        return Vector<3, T>(x * vec.x, y * vec.y, z * vec.z);
    }

    Vector<3, T> operator*(const T& scl) const
    {
        return Vector<3, T>(x * scl, y * scl, z * scl);
    }

    Vector<3, T> operator/(const Vector<3, T> vec) const
    {
        return Vector<3, T>(x / vec.x, y / vec.y, z / vec.z);
    }

    Vector<3, T> operator/(const T& scl) const
    {
        return Vector<3, T>(x / scl, y / scl, z / scl);
    }

    Vector<3, T>& operator+=(const Vector<3, T> vec)
    {
        *this = *this + vec;
        return *this;
    }

    Vector<3, T>& operator+=(const T& num)
    {
        x += num;
        y += num;
        z += num;
        return *this;
    }

    Vector<3, T>& operator-=(const Vector<3, T> vec)
    {
        *this = *this - vec;
        return *this;
    }

    Vector<3, T>& operator-=(const T& num)
    {
        x -= num;
        y -= num;
        z -= num;
        return *this;
    }

    Vector<3, T>& operator*=(const Vector<3, T> vec)
    {
        *this = *this * vec;
        return *this;
    }

    Vector<3, T>& operator*=(const T& scl)
    {
        x *= scl;
        y *= scl;
        z *= scl;
        return *this;
    }

    Vector<3, T>& operator/=(const Vector<3, T> vec)
    {
        *this = *this / vec;
        return *this;
    }

    Vector<3, T>& operator/=(const T& scl)
    {
        x /= scl;
        y /= scl;
        z /= scl;
        return *this;
    }

    std::string str() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }

    T x, y, z;
};

typedef Vector<3, float>        Vector3f;
typedef Vector<3, int>          Vector3i;
typedef Vector<3, long>         Vector3l;
typedef Vector<3, double>       Vector3d;
typedef Vector<3, unsigned int> Vector3u;