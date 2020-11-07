#pragma once

#include <ostream>

template<int length, typename T>
class Vector;

template <typename T>
class Vector<2, T>
{
public:
    Vector<2, T>()
        : x(0), y(0)
    {
    }

    Vector<2, T>(T x, T y)
        : x(x), y(y)
    {
    }

    Vector<2, T>(T v)
        : x(v), y(v)
    {
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
    
    bool operator==(const Vector<2, T>& vec)
    {
        return x == vec.x && y == vec.y;
    }

    bool operator!=(const Vector<2, T>& vec)
    {
        return !operator==(vec);
    }

    float mag()
    {
        return sqrt(x * x, y * y);
    }

    void normalize()
    {
        Vector<2, T>::normalize(*this);
    }

    float dot(const Vector<2, T>& vec)
    {
        return Vector<2, T>::dot(*this, vec);
    }

    static Vector<2, T> normalize(const Vector<2, T>& vec)
    {
        float magnitude = 1.f / mag(); // Divide once (save CPU cycles)
        return Vector<2, T>(vec.x * magnitude, vec.y * magnitude);
    }

    static float dot(const Vector<2, T>& vec1, const Vector<2, T>& vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
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