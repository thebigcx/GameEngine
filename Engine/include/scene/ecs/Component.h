#pragma once

#include <maths/math.h>

#include <util/Transform.h>

struct Component
{

};

struct TransformComponent : public Component
{
    math::vec3 translation;
    math::vec3 rotation;
    math::vec3 scale;

    math::mat4 getTransform()
    {
        Transform transform = { translation, rotation, scale };
        return transform.matrix();
    }
};