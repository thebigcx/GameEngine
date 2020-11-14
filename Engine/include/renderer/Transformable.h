#pragma once

#include <util/Transform.h>
#include <math/vector/vec2.h>
#include <core/Console.h>

class Transformable
{
public:
    Transformable() {};

    virtual ~Transformable() = default;

    void setPosition(math::vec2 position)
    {
        m_position = position;
    }

    void setSize(math::vec2 size)
    {
        m_size = size;
    }

    void setRotation(float rotation)
    {
        m_rotation = rotation;
    }

    void setOrigin(math::vec2 origin)
    {
        m_origin = origin;
    }

    void translate(math::vec2 direction)
    {
        m_position += direction;
    }

    void rotate(float rotation)
    {
        m_rotation += rotation;
    }

    void scale(math::vec2 scalar)
    {
        m_size *= scalar;
    }

    const math::vec2& getPosition() const { return m_position; }
    const math::vec2& getSize()     const { return m_size;     }
    float           getRotation() const { return m_rotation; }
    const math::vec2& getOrigin()   const { return m_origin;   }

    Transform getTransform() const
    {
        return { m_position, m_rotation, m_size, m_origin };
    }

protected:
    math::vec2 m_position;
    math::vec2 m_size;
    math::vec2 m_origin;
    float m_rotation = 0;
};