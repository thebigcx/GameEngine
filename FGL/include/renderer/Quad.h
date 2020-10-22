#pragma once

#include <cstdint>
#include <array>

#include <util/maths/Vector2.h>
#include <util/maths/Rect.h>
#include <util/Color.h>
#include <renderer/Vertex.h>
#include <renderer/Transformable.h>

class Quad : public Transformable
{
public:
    Quad();
    Quad(const Vector2f& position, const Vector2f& size, const Color& color);

    void setTexture2DRect(const FloatRect& rect);
    void setColor(const Color& color);

    std::array<Vertex, 4> getVertices() const;

    static inline int getVertexCount() { return 4; }
    static inline const std::array<unsigned int, 6>& getIndices() { return m_indices; }

private:
    Color m_color;

    FloatRect m_texRect;

    static inline const std::array<unsigned int, 6> m_indices = {
        0, 1, 2,
        0, 2, 3
    };

    static inline const std::array<Vector2f, 4> m_positions = {
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1)
    };
};