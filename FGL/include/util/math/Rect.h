#pragma once

template <typename T>
class Rect
{
public:
	Rect() {}

	Rect(T x, T y, T width, T height)
		: x(x), y(y), width(width), height(height)
	{
	}

	Rect(const Vector2<T>& pos, const Vector2<T>& size)
		: x(pos.x), y(pos.y), width(size.x), height(size.y)
	{
	}

	bool contains(const Vector2<T>& point) const
	{
		return point.x > x && point.x < x + width && point.y > y && point.y < y + height;
	}

	bool intersects(const Rect<T>& other) const
	{
		return other.x < x + width && other.x + other.width > x &&
			   other.y < y + height && other.y + other.height > y;
	}

	const Vector2<T> getPosition() const
	{
		return Vector2f(x, y);
	}

	Vector2<T> getSize() const
	{
		return Vector2f(width, height);
	}

	Vector2<T> getIntersection(Rect<T> other) const
	{
		Vector2<T> amount;
		amount.x = abs(other.x - x + width);
		amount.y = abs(other.y - y + height);
		return amount;
	}

	T x, y, width, height;
};

typedef Rect<int> IntRect;
typedef Rect<float> FloatRect;