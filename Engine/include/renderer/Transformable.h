#pragma once

#include <util/Transform.h>
#include <math/vector/Vector2.h>
#include <core/Console.h>

class Transformable
{
public:
    Transformable() {};

    virtual ~Transformable() = default;

    void setPosition(math::Vector2f position)
    {
        m_position = position;
    }

    void setSize(math::Vector2f size)
    {
        m_size = size;
    }

    void setRotation(float rotation)
    {
        m_rotation = rotation;
    }

    void setOrigin(math::Vector2f origin)
    {
        m_origin = origin;
    }

    void translate(math::Vector2f direction)
    {
        m_position += direction;
    }

    void rotate(float rotation)
    {
        m_rotation += rotation;
    }

    void scale(math::Vector2f scalar)
    {
        m_size *= scalar;
    }

    const math::Vector2f& getPosition() const { return m_position; }
    const math::Vector2f& getSize()     const { return m_size;     }
    float           getRotation() const { return m_rotation; }
    const math::Vector2f& getOrigin()   const { return m_origin;   }

    Transform getTransform() const
    {
        return Transform(m_position, m_rotation, m_size, m_origin);
    }

protected:
    math::Vector2f m_position;
    math::Vector2f m_size;
    math::Vector2f m_origin;
    float m_rotation = 0;
};