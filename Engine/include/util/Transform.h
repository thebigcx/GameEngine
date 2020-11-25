#pragma once

#include <iostream>

#include <maths/maths/vector/vec2.h>
#include <maths/maths/matrix/mat4.h>
#include <maths/maths/math.h>
#include <maths/maths/matrix/matrix_transform.h>

struct Transform
{
    math::vec2 position;
    float rotation = 0.f;
    math::vec2 scale = math::vec2(1.f, 1.f);
    math::vec2 origin;

    math::mat4 matrix() const
    {
        math::mat4 mat(1.f);

        mat = math::translate(mat, math::vec3(position, 0));
        
        mat = math::translate(mat, math::vec3(origin.x, origin.y, 0.f));
        mat = math::rotate(mat, (float)math::asRadians(rotation), math::vec3(0, 0, 1));
        mat = math::translate(mat, math::vec3(-origin.x, -origin.y, 0.f));

        mat = math::scale(mat, math::vec3(scale, 1));

        return mat;
    }
};