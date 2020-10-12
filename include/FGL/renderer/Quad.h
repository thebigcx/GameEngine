#pragma once

#include <cstdint>
#include <array>

#include "../util/maths/Vector2.h"
#include "../util/Color.h"
#include "Vertex.h"
#include "../Transformable.h"

class Quad : public Transformable
{
public:
    Quad(const Vector2f& position, const Vector2f& size, const Color& color);

    inline const Vector2f& getPosition() const
    {
        return m_position;
    }

    void move(Vector2f direction);

    std::array<Vertex, 4> getVertices() const;

    static inline int getVertexCount() { return 4; }

    friend class Batch;

private:
    Color m_color;

    static inline const std::array<unsigned int, 6> m_indices = {
        0, 1, 2,
        0, 2, 3
    };

    Vector2f m_positions[4];
    Color m_colors[4];
};