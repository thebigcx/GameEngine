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
    Quad(const Vector2f& position, const Vector2f& size);
    Quad(const Vector2f& position, const Vector2f& size, const Color& color);
    Quad(const FloatRect& rect);
    Quad(float x, float y, float width, float height);

    void setTextureRect(const FloatRect& rect);
    void setColor(const Color& color);

    inline const Color& getColor() const { return m_color; }
    inline const FloatRect& getTextureRect() const { return m_texRect; }

    static inline int getVertexCount() { return 4; }    

    static inline const std::array<unsigned int, 6> indices = {
        0, 1, 2,
        2, 3, 0
    };

    static inline const std::array<Vector2f, 4> positions = {
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1)
    };

private:
    Color m_color;

    FloatRect m_texRect;

    
};