#pragma once

#include <iostream>

#include <math/vector/Vector2.h>
#include <math/matrix/Matrix4.h>
#include <math/Math.h>
#include <math/matrix/MatrixTransform.h>

class Transform
{
public:
    Transform() 
        : scale(1.f) {}

    ~Transform() {}

    Transform(const math::Vector2f& position_) 
        : position(position_) {}

    Transform(const math::Vector2f& position_, float rotation_)
        : position(position_), rotation(rotation_) {}

    Transform(const math::Vector2f& position_, float rotation_, const math::Vector2f& scale_) 
        : position(position_), rotation(rotation_), scale(scale_) {}

    Transform(const math::Vector2f& position_, float rotation_, const math::Vector2f& scale_, const math::Vector2f& origin_)
        : position(position_), rotation(rotation_), scale(scale_), origin(origin_) {}

    Transform(const math::Vector2f& position_, const math::Vector2f& scale_)
        : position(position_), scale(scale_) {}

    math::Matrix4f matrix() const
    {
        math::Matrix4f mat(1.f);

        mat = math::translate(mat, math::Vector3f(position, 0));
        
        mat = math::translate(mat, math::Vector3f(origin.x, origin.y, 0.f));
        mat = math::rotate(mat, (float)math::asRadians(rotation), math::Vector3f(0, 0, 1));
        mat = math::translate(mat, math::Vector3f(-origin.x, -origin.y, 0.f));

        mat = math::scale(mat, math::Vector3f(scale, 1));

        return mat;
    }

    math::Vector2f position;
    float rotation;
    math::Vector2f origin;
    math::Vector2f scale;

    static Transform None;

private:
    math::Matrix4f m_matrix;
};