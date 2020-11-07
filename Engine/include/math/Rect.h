#pragma once

#include <math/vector/Vector2.h>

template <typename T>
class Rect
{
public:
	Rect() 
		: x(0), y(0), width(0), height(0)
	{
	}

	Rect(T x, T y, T width, T height)
		: x(x), y(y), width(width), height(height)
	{
	}

	Rect(const Vector<2, T>& pos, const Vector<2, T>& size)
		: x(pos.x), y(pos.y), width(size.x), height(size.y)
	{
	}

	bool contains(const Vector<2, T>& point) const
	{
		return point.x > x && point.x < x + width && point.y > y && point.y < y + height;
	}

	bool intersects(const Rect<T>& other) const
	{
		return other.x < x + width && other.x + other.width > x &&
			   other.y < y + height && other.y + other.height > y;
	}

	Vector<2, T> getPosition() const
	{
		return Vector<2, T>(x, y);
	}

	Vector<2, T> getSize() const
	{
		return Vector<2, T>(width, height);
	}

	Vector<2, T> getIntersection(Rect<T> other) const
	{
		Vector<2, T> amount;
		amount.x = abs(other.x - x + width);
		amount.y = abs(other.y - y + height);
		return amount;
	}

	T x, y, width, height;
};

typedef Rect<int>          IntRect;
typedef Rect<float>        FloatRect;
typedef Rect<double>       DoubleRect;
typedef Rect<long>         LongRect;
typedef Rect<unsigned int> UnsignedRect;