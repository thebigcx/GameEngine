#pragma once

#include <iostream>

#include <maths/vector/vec2.h>
#include <maths/matrix/mat4x4.h>
#include <maths/math.h>
#include <maths/matrix/matrix_transform.h>

struct Transform
{
    math::vec3 translation;
    math::vec3 rotation = 0.f;
    math::vec3 scale = math::vec3(1.f);
    math::vec3 origin;

    math::mat4 matrix() const
    {
        math::mat4 mat(1.f);

        mat = math::translate(mat, translation);
        
        mat = math::translate(mat, origin);

        mat = math::rotate(mat, (float)math::radians(rotation.x), math::vec3(1, 0, 0));
        mat = math::rotate(mat, (float)math::radians(rotation.y), math::vec3(0, 1, 0));
        mat = math::rotate(mat, (float)math::radians(rotation.z), math::vec3(0, 0, 1));

        mat = math::translate(mat, math::vec3(-origin.x, -origin.y, -origin.z));

        mat = math::scale(mat, scale);

        return mat;
    }
};