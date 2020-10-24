#pragma once

#include <ostream>

template <typename T>
class Vector2
{
public:
    Vector2()
    {
        x = 0;
        y = 0;
    }

    Vector2(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

    Vector2(T v)
    {
        x = v;
        y = v;
    }

    Vector2<T> operator+(const Vector2<T> vec) const
    {
        return Vector2<T>(x + vec.x, y + vec.y);
    }

    Vector2<T> operator+(const T& num) const
    {
        return Vector2<T>(x + num, y + num);
    }

    Vector2<T> operator-(const Vector2<T> vec) const
    {
        return Vector2<T>(x - vec.x, y - vec.y);
    }

    Vector2<T> operator-(const T& num) const
    {
        return Vector2<T>(x - num, y - num);
    }

    Vector2<T> operator*(const Vector2<T> vec) const
    {
        return Vector2<T>(x * vec.x, y * vec.y);
    }

    Vector2<T> operator*(const T& scl) const
    {
        return Vector2<T>(x * scl, y * scl);
    }

    Vector2<T> operator/(const Vector2<T> vec) const
    {
        return Vector2<T>(x / vec.x, y / vec.y);
    }

    Vector2<T> operator/(const T& scl) const
    {
        return Vector2<T>(x / scl, y / scl);
    }

    Vector2<T>& operator+=(const Vector2<T> vec)
    {
        *this = *this + vec;
        return *this;
    }

    Vector2<T>& operator+=(const T& num)
    {
        x = x + num;
        y = y + num;
        return *this;
    }

    Vector2<T>& operator-=(const Vector2<T> vec)
    {
        *this = *this - vec;
        return *this;
    }

    Vector2<T>& operator-=(const T& num)
    {
        x = x - num;
        y = y - num;
        return *this;
    }

    Vector2<T>& operator*=(const Vector2<T> vec)
    {
        *this = *this * vec;
        return *this;
    }

    Vector2<T>& operator*=(const T& scl)
    {
        x = x * scl;
        y = y * scl;
        return *this;
    }

    Vector2<T>& operator/=(const Vector2<T> vec)
    {
        *this = *this / vec;
        return *this;
    }

    Vector2<T>& operator/=(const T& scl)
    {
        x = x / scl;
        y = y / scl;
        return *this;
    }

    std::string str() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    T x, y;
};

typedef Vector2<float>        Vector2f;
typedef Vector2<int>          Vector2i;
typedef Vector2<long>         Vector2l;
typedef Vector2<double>       Vector2d;
typedef Vector2<unsigned int> Vector2u;