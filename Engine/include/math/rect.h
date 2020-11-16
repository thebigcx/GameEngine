#pragma once

#include <math/vector/vec2.h>

namespace math
{

template<typename T>
class rect
{
public:
	rect() 
		: x(0), y(0), width(0), height(0)
	{
	}

	rect(T x, T y, T width, T height)
		: x(x), y(y), width(width), height(height)
	{
	}

	rect(const math::vec<2, T>& pos, const math::vec<2, T>& size)
		: x(pos.x), y(pos.y), width(size.x), height(size.y)
	{
	}

	bool contains(const math::vec<2, T>& point) const
	{
		return point.x > x && point.x < x + width && point.y > y && point.y < y + height;
	}

	bool intersects(const rect<T>& other) const
	{
		return other.x < x + width && other.x + other.width > x &&
			   other.y < y + height && other.y + other.height > y;
	}

	math::vec<2, T> getPosition() const
	{
		return math::vec<2, T>(x, y);
	}

	math::vec<2, T> getSize() const
	{
		return math::vec<2, T>(width, height);
	}

	math::vec<2, T> getIntersection(rect<T> other) const
	{
		math::vec<2, T> amount;
		amount.x = abs(other.x - x + width);
		amount.y = abs(other.y - y + height);
		return amount;
	}

	bool operator==(const rect<T>& rect)
	{
		return x == rect.x && y == rect.y && width == rect.width && height == rect.height;
	}

	bool operator!=(const rect<T>& rect)
	{
		return !operator==(rect);
	}

	T x, y, width, height;
};

typedef rect<int>          irect;
typedef rect<float>        frect;
typedef rect<double>       drect;
typedef rect<long>         lrect;
typedef rect<unsigned int> urect;

}