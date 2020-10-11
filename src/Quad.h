#pragma once

#include <cstdint>

#include "util/maths/Vector2.h"

class Quad
{
public:
    Quad(Vector2f position);

    friend class Batch;

private:
    static inline const unsigned int m_indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    static inline const float m_positions[] = {
        0.f, 0.f,
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f
    };
};