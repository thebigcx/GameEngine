#pragma once

#include <iostream>

#include <maths/vector/vec2.h>
#include <maths/matrix/mat4x4.h>
#include <maths/math.h>
#include <maths/matrix/matrix_transform.h>

struct Transform
{
    math::vec3 translation;
    math::vec3 rotation;
    math::vec3 scale = math::vec3(1.f);

    math::mat4 matrix() const
    {
        return math::translate(math::mat4(1.f), translation)
             * math::to_mat4(math::quat(math::radians(rotation)))
             * math::scale(math::mat4(1.f), scale);
    }
};