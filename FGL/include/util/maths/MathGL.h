#pragma once

namespace MathGL
{

const float* makeRaw(const Vector2f& vec)
{
    return &(vec.x);
}

float* makeRaw(Vector2f& vec)
{
    return &(vec.x);
}

}