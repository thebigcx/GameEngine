#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/maths/Vector2.h"
#include "core/Logger.h"

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

    const Vector2f& getOrigin() const
    {
        return m_origin;
    }

    glm::mat4 getTransform() const
    {
        glm::mat4 matrix(1.f);

        Vector2f pos(m_position.x, m_position.y);
        Vector2f scalar(m_size.x, m_size.y);

        // Translate
        matrix = glm::translate(matrix, glm::vec3(pos.x, pos.y, 0));
        // Rotate (with respect to origin)
        matrix = glm::translate(matrix, glm::vec3(m_origin.x, m_origin.y, 0.f));
        matrix = glm::rotate(matrix, glm::radians((float)m_rotation), glm::vec3(0.f, 0.f, 1.f));
        matrix = glm::translate(matrix, glm::vec3(-m_origin.x, -m_origin.y, 0.f));

        // Scale
        matrix = glm::scale(matrix, glm::vec3(scalar.x, scalar.y, 1));

        return matrix;
    }

protected:
    Vector2f m_position;
    Vector2f m_size;
    Vector2f m_origin;
    float m_rotation = 0;
};