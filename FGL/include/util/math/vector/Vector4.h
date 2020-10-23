#pragma once

#include <ostream>

#include <util/math/vector/Vector3.h>

template <typename T>
class Vector4
{
public:
    Vector4()
    {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }

    Vector4(T x, T y, T z, T w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    Vector4(T v, T w)
    {
        x = v;
        y = v;
        z = v;
        w = w;
    }

    Vector4(Vector3<T> vec, T w)
    {
        this->x = vec.x;
        this->y = vec.y;
        this->z = vec.z;
        this->w = w;
    }

    Vector4<T> operator+(const Vector4<T> vec) const
    {
        return Vector4<T>(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
    }

    Vector4<T> operator+(const T& num) const
    {
        return Vector4<T>(x + num, y + num, z + num, w + num);
    }

    Vector4<T> operator-(const Vector4<T> vec) const
    {
        return Vector4<T>(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
    }

    Vector4<T> operator-(const T& num) const
    {
        return Vector4<T>(x - num, y - num, z - num, w - num);
    }

    Vector4<T> operator*(const Vector4<T> vec) const
    {
        return Vector4<T>(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
    }

    Vector4<T> operator*(const T& scl) const
    {
        return Vector4<T>(x * scl, y * scl, z * scl, w * scl);
    }

    Vector4<T> operator/(const Vector4<T> vec) const
    {
        return Vector4<T>(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
    }

    Vector4<T> operator/(const T& scl) const
    {
        return Vector4<T>(x / scl, y / scl, z / scl, w / scl);
    }

    Vector4<T>& operator+=(const Vector4<T> vec)
    {
        *this = *this + vec;
        return *this;
    }

    Vector4<T>& operator+=(const T& num)
    {
        *this = *this + num;
        return *this;
    }

    Vector4<T>& operator-=(const Vector4<T> vec)
    {
        *this = *this - vec;
        return *this;
    }

    Vector4<T>& operator-=(const T& num)
    {
        *this = *this - num;
        return *this;
    }

    Vector4<T>& operator*=(const Vector4<T> vec)
    {
        *this = *this * vec;
        return *this;
    }

    Vector4<T>& operator*=(const T& scl)
    {
        *this = *this * scl;
        return *this;
    }

    Vector4<T>& operator/=(const Vector4<T> vec)
    {
        *this = *this / vec;
        return *this;
    }

    Vector4<T>& operator/=(const T& scl)
    {
        *this = *this / scl;
        return *this;
    }

    std::string str() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
    }

    T x, y, z, w;
};

typedef Vector4<float>        Vector4f;
typedef Vector4<int>          Vector4i;
typedef Vector4<long>         Vector4l;
typedef Vector4<double>       Vector4d;
typedef Vector4<unsigned int> Vector4u;