#pragma once

#include <maths/math.h>

#include <util/Transform.h>

struct TransformComponent
{
    TransformComponent(const math::vec3& translation, const math::vec3& rotation, const math::vec3& scale)
        : translation(translation), rotation(rotation), scale(scale) {}

    math::vec3 translation;
    math::vec3 rotation;
    math::vec3 scale;

    math::mat4 getTransform()
    {
        Transform transform = { translation, rotation, scale };
        return transform.matrix();
    }
};

struct TagComponent
{
    TagComponent(const std::string& str)
        : tag(str) {}
        
    std::string tag;
};