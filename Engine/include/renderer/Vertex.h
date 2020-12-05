#pragma once

#include <maths/vector/vec2.h>
#include <maths/vector/vec4.h>

struct Vertex
{
    math::vec3 position;
    math::vec2 texCoord;
    math::vec4 color;
    float texIndex;
};