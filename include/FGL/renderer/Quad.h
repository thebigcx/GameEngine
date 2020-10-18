#pragma once

#include <cstdint>
#include <array>

#include "../util/maths/Vector2.h"
#include "../util/maths/Rect.h"
#include "../util/Color.h"
#include "Vertex.h"
#include "../Transformable.h"

class Quad : public Transformable
{
public:
    Quad(const Vector2f& position, const Vector2f& size, const Color& color);

    void move(Vector2f direction);

    std::array<Vertex, 4> getVertices() const;

    static inline int getVertexCount() { return 4; }

    void setTextureRect(const FloatRect& rect);

    void setColor(const Color& color);

    static inline const std::array<unsigned int, 6> getIndices()
    {
        return m_indices;
    }

private:
    Color m_color;

    static inline const std::array<unsigned int, 6> m_indices = {
        0, 1, 2,
        0, 2, 3
    };

    FloatRect m_texRect;
};