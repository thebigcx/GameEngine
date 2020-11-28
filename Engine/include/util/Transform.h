#pragma once

#include <iostream>

#include <maths/vector/vec2.h>
#include <maths/matrix/mat4x4.h>
#include <maths/math.h>
#include <maths/matrix/matrix_transform.h>

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
        mat = math::rotate(mat, (float)math::radians(rotation), math::vec3(0, 0, 1));
        mat = math::translate(mat, math::vec3(-origin.x, -origin.y, 0.f));

        mat = math::scale(mat, math::vec3(scale, 1));

        return mat;
    }
};