#pragma once

#include <cstdint>
#include <array>

#include <util/math/vector/Vector2.h>
#include <util/math/Rect.h>
#include <util/Color.h>
#include <renderer/Vertex.h>
#include <renderer/Transformable.h>
#include <renderer/Texture2D.h>

class Quad : public Transformable
{
public:
    Quad();
    Quad(const Texture2D& texture, const Vector2f& position, const Vector2f& size, const Color& color);

    void setTextureRect(const FloatRect& rect);
    void setColor(const Color& color);
    void setTexture(const Texture2D& texture);

    std::array<Vertex, 4> getVertices() const;

    static inline int getVertexCount() { return 4; }
    static inline const std::array<unsigned int, 6>& getIndices() { return m_indices; }

private:
    Color m_color;

    FloatRect m_texRect;

    const Texture2D* m_pTexture;

    static inline const std::array<unsigned int, 6> m_indices = {
        0, 1, 2,
        2, 3, 0
    };

    static inline const std::array<Vector2f, 4> m_positions = {
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1)
    };
};