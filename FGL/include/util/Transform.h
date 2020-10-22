#pragma once

#include <util/maths/Vector2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
    Transform() {}

    ~Transform() {}

    Transform(const Vector2f& position_) 
        : position(position_)
    {}

    Transform(const Vector2f& position_, float rotation_)
        : position(position_), rotation(rotation_)
    {}

    Transform(const Vector2f& position_, float rotation_, const Vector2f& scale_) 
        : position(position_), rotation(rotation_), scale(scale_)
    {}

    Transform(const Vector2f& position_, const Vector2f& scale_)
        : position(position_), scale(scale_)
    {}


    glm::mat4 getMatrix()
    {
        glm::mat4 matrix(1.f);

        Vector2f pos(position.x, position.y);
        Vector2f scalar(scale.x, scale.y);

        // Translate
        matrix = glm::translate(matrix, glm::vec3(pos.x, pos.y, 0));
        // Rotate (with respect to origin)
        matrix = glm::translate(matrix, glm::vec3(rotationOrigin.x, rotationOrigin.y, 0.f));
        matrix = glm::rotate(matrix, glm::radians((float)rotation), glm::vec3(0.f, 0.f, 1.f));
        matrix = glm::translate(matrix, glm::vec3(-rotationOrigin.x, -rotationOrigin.y, 0.f));

        // Scale
        matrix = glm::scale(matrix, glm::vec3(scalar.x, scalar.y, 1));
        return matrix;
    }

    Vector2f position;
    float rotation;
    Vector2f rotationOrigin;
    Vector2f scale;

private:
    glm::mat4 m_matrix;
};