#pragma once

#include <ostream>

#include <util/math/vector/Vector2.h>

template <typename T>
class Vector3
{
public:
    Vector3()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    Vector3(T x, T y, T z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3(T v)
    {
        x = v;
        y = v;
        z = v;
    }

    Vector3(Vector2<T> vec, T z)
    {
        this->x = vec.x;
        this->y = vec.y;
        this->z = z;
    }

    Vector3<T> operator+(const Vector3<T> vec) const
    {
        return Vector3<T>(x + vec.x, y + vec.y, z + vec.z);
    }

    Vector3<T> operator+(const T& num) const
    {
        return Vector3<T>(x + num, y + num, z + num);
    }

    Vector3<T> operator-(const Vector3<T> vec) const
    {
        return Vector3<T>(x - vec.x, y - vec.y, z - vec.z);
    }

    Vector3<T> operator-(const T& num) const
    {
        return Vector3<T>(x - num, y - num, z - num);
    }

    Vector3<T> operator*(const Vector3<T> vec) const
    {
        return Vector3<T>(x * vec.x, y * vec.y, z * vec.z);
    }

    Vector3<T> operator*(const T& scl) const
    {
        return Vector3<T>(x * scl, y * scl, z * scl);
    }

    Vector3<T> operator/(const Vector3<T> vec) const
    {
        return Vector3<T>(x / vec.x, y / vec.y, z / vec.z);
    }

    Vector3<T> operator/(const T& scl) const
    {
        return Vector3<T>(x / scl, y / scl, z / scl);
    }

    Vector3<T>& operator+=(const Vector3<T> vec)
    {
        *this = *this + vec;
        return *this;
    }

    Vector3<T>& operator+=(const T& num)
    {
        x += num;
        y += num;
        z += num;
        return *this;
    }

    Vector3<T>& operator-=(const Vector3<T> vec)
    {
        *this = *this - vec;
        return *this;
    }

    Vector3<T>& operator-=(const T& num)
    {
        x -= num;
        y -= num;
        z -= num;
        return *this;
    }

    Vector3<T>& operator*=(const Vector3<T> vec)
    {
        *this = *this * vec;
        return *this;
    }

    Vector3<T>& operator*=(const T& scl)
    {
        x *= scl;
        y *= scl;
        z *= scl;
        return *this;
    }

    Vector3<T>& operator/=(const Vector3<T> vec)
    {
        *this = *this / vec;
        return *this;
    }

    Vector3<T>& operator/=(const T& scl)
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

typedef Vector3<float>        Vector3f;
typedef Vector3<int>          Vector3i;
typedef Vector3<long>         Vector3l;
typedef Vector3<double>       Vector3d;
typedef Vector3<unsigned int> Vector3u;