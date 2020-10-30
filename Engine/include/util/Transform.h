#pragma once

#include <iostream>

#include <util/math/vector/Vector2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <util/math/matrix/Matrix4.h>
#include <util/math/Math.h>

class Transform
{
public:
    Transform() 
        : scale(1.f)
    {}

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

    Matrix4f getMatrix() const
    {
        Matrix4f mat(1.f);

        mat.translate(Vector3f(position, 0));
        
        mat.translate(Vector3f(rotationOrigin.x, rotationOrigin.y, 0.f));
        mat.rotate(asRadians(rotation), Vector3f(0, 0, 1));
        mat.translate(Vector3f(-rotationOrigin.x, -rotationOrigin.y, 0.f));

        mat.scale(Vector3f(scale, 1));

        return mat;
    }

    Vector2f position;
    float rotation;
    Vector2f rotationOrigin;
    Vector2f scale;

    static Transform None;

private:
    //glm::mat4 m_matrix;
    Matrix4f m_matrix;
};