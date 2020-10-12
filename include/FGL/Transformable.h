#pragma once

#include "util/maths/Vector2.h"

class Transformable
{
public:
    Transformable() {};

    virtual ~Transformable() = default;

    void setPosition(Vector2f position)
    {
        m_position = position;
    }

    void setSize(Vector2f size)
    {
        m_size = size;
    }

    void setRotation(float rotation)
    {
        m_rotation = rotation;
    }

    void move(Vector2f direction)
    {
        m_position += direction;
    }

    void rotate(float rotation)
    {
        m_rotation += rotation;
    }

    void scale(Vector2f scalar)
    {
        m_size *= scalar;
    }

    const Vector2f& getPosition() const
    {
        return m_position;
    }

    const Vector2f& getSize() const
    {
        return m_size;
    }

    float getRotation() const
    {
        return m_rotation;
    }

protected:
    Vector2f m_position;
    Vector2f m_size;
    float m_rotation;
};