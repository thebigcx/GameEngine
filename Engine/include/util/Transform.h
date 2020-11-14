#pragma once

#include <iostream>

#include <math/vector/Vector2.h>
#include <math/matrix/Matrix4.h>
#include <math/Math.h>
#include <math/matrix/MatrixTransform.h>

struct Transform
{
    math::Vector2f position;
    float rotation = 0.f;
    math::Vector2f scale = math::Vector2f(1.f, 1.f);
    math::Vector2f origin;

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
};