#pragma once

#include <cstdint>
#include <array>

#include "../util/maths/Vector2.h"
#include "../util/Color.h"
#include "Vertex.h"

class Quad
{
public:
    Quad(const Vector2f& position, const Vector2f& size, const Color& color);

    const Vector2f& getPosition() const
    {
        return m_position;
    }

    void move(Vector2f direction);

    std::array<Vertex, 4> getVertices() const;

    friend class Batch;

private:
    Vector2f m_position;
    Vector2f m_size;
    Color m_color;

    static inline const unsigned int m_indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    Vector2f m_positions[4];
    Color m_colors[4];
};