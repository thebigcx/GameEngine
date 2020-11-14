#pragma once

#include <cstdint>
#include <array>

#include <math/vector/vec2.h>
#include <math/Rect.h>
#include <renderer/Vertex.h>
#include <renderer/Transformable.h>
#include <renderer/Texture2D.h>

class Sprite : public Transformable
{
public:
    Sprite();
    Sprite(const math::vec2& position, const math::vec2& size);
    Sprite(const math::vec2& position, const math::vec2& size, const math::vec4& color);
    Sprite(const FloatRect& rect);
    Sprite(float x, float y, float width, float height);

    void setTextureRect(const FloatRect& rect);
    void setColor(const math::vec4& color);

    inline const math::vec4& getColor() const { return m_color; }
    inline const FloatRect& getTextureRect() const { return m_texRect; }

    static inline int getVertexCount() { return 4; }    

    static inline const std::array<unsigned int, 6> indices = {
        0, 1, 2,
        2, 3, 0
    };

    static inline const std::array<math::vec2, 4> positions = {
        math::vec2(0, 0),
        math::vec2(1, 0),
        math::vec2(1, 1),
        math::vec2(0, 1)
    };

private:
    math::vec4 m_color;

    FloatRect m_texRect;

    
};