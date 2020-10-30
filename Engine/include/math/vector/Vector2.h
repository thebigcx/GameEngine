#pragma once

#include <ostream>

template<int length, typename T>
class Vector;

template <typename T>
class Vector<2, T>
{
public:
    Vector<2, T>()
    {
        x = 0;
        y = 0;
    }

    Vector<2, T>(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

    Vector<2, T>(T v)
    {
        x = v;
        y = v;
    }

    Vector<2, T> operator+(const Vector<2, T> vec) const
    {
        return Vector<2, T>(x + vec.x, y + vec.y);
    }

    Vector<2, T> operator+(const T& num) const
    {
        return Vector<2, T>(x + num, y + num);
    }

    Vector<2, T> operator-(const Vector<2, T> vec) const
    {
        return Vector<2, T>(x - vec.x, y - vec.y);
    }

    Vector<2, T> operator-(const T& num) const
    {
        return Vector<2, T>(x - num, y - num);
    }

    Vector<2, T> operator*(const Vector<2, T> vec) const
    {
        return Vector<2, T>(x * vec.x, y * vec.y);
    }

    Vector<2, T> operator*(const T& scl) const
    {
        return Vector<2, T>(x * scl, y * scl);
    }

    Vector<2, T> operator/(const Vector<2, T> vec) const
    {
        return Vector<2, T>(x / vec.x, y / vec.y);
    }

    Vector<2, T> operator/(const T& scl) const
    {
        return Vector<2, T>(x / scl, y / scl);
    }

    Vector<2, T>& operator+=(const Vector<2, T> vec)
    {
        *this = *this + vec;
        return *this;
    }

    Vector<2, T>& operator+=(const T& num)
    {
        x = x + num;
        y = y + num;
        return *this;
    }

    Vector<2, T>& operator-=(const Vector<2, T> vec)
    {
        *this = *this - vec;
        return *this;
    }

    Vector<2, T>& operator-=(const T& num)
    {
        x = x - num;
        y = y - num;
        return *this;
    }

    Vector<2, T>& operator*=(const Vector<2, T> vec)
    {
        *this = *this * vec;
        return *this;
    }

    Vector<2, T>& operator*=(const T& scl)
    {
        x = x * scl;
        y = y * scl;
        return *this;
    }

    Vector<2, T>& operator/=(const Vector<2, T> vec)
    {
        *this = *this / vec;
        return *this;
    }

    Vector<2, T>& operator/=(const T& scl)
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

typedef Vector<2, float>        Vector2f;
typedef Vector<2, int>          Vector2i;
typedef Vector<2, long>         Vector2l;
typedef Vector<2, double>       Vector2d;
typedef Vector<2, unsigned int> Vector2u;