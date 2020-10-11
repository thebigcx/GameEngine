#pragma once

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

    Vector2<T> operator+(const Vector2<T> vec)
    {
        return Vector2<T>(x + vec.x, y + vec.y);
    }

    Vector2<T> operator-(const Vector2<T> vec)
    {
        return Vector2<T>(x - vec.x, y - vec.y);
    }

    Vector2<T> operator*(const Vector2<T> vec)
    {
        return Vector2<T>(x * vec.x, y * vec.y);
    }

    Vector2<T> operator/(const Vector2<T> vec)
    {
        return Vector2<T>(x / vec.x, y / vec.y);
    }

    Vector2<T>& operator+=(const Vector2<T> vec)
    {
        x = x + vec.x;
        y = y + vec.y;
        return *this;
    }

    Vector2<T>& operator-=(const Vector2<T> vec)
    {
        x = x - vec.x;
        y = y - vec.y;
        return *this;
    }

    Vector2<T>& operator*=(const Vector2<T> vec)
    {
        x = x * vec.x;
        y = y * vec.y;
        return *this;
    }

    Vector2<T>& operator/=(const Vector2<T> vec)
    {
        x = x / vec.x;
        y = y / vec.y;
        return *this;
    }

    T x, y;
};

typedef Vector2<float>        Vector2f;
typedef Vector2<int>          Vector2i;
typedef Vector2<long>         Vector2l;
typedef Vector2<double>       Vector2d;
typedef Vector2<unsigned int> Vector2u;