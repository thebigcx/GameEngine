#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <util/Transform.h>
#include <util/maths/Vector2.h>
#include <core/Logger.h>

class Transformable
{
public:
    Transformable() {};

    virtual ~Transformable() = default;

    void setPosition(Vector2f position) {
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

    void setOrigin(Vector2f origin)
    {
        m_origin = origin;
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

    const Vector2f& getPosition() const { return m_position; }
    const Vector2f& getSize()     const { return m_size;     }
    float           getRotation() const { return m_rotation; }
    const Vector2f& getOrigin()   const { return m_origin;   }

    Transform getTransform() const
    {
        Transform transform(m_position, m_rotation, m_size);
        transform.rotationOrigin = m_origin;
        return transform;
    }

protected:
    Vector2f m_position;
    Vector2f m_size;
    Vector2f m_origin;
    float m_rotation = 0;
};