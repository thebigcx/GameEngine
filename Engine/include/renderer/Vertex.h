#pragma once

#include <math/vector/Vector2.h>
#include <util/Color.h>

struct Vertex
{
    math::Vector2f position;
    math::Vector2f texCoord;
    Color color;
};