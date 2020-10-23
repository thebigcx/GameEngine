#pragma once

#include <util/math/vector/Vector2.h>

class Glyph
{
public:
    Glyph() {}
        
    Glyph(unsigned int texture, Vector2i size, Vector2i bearing, unsigned int advance);

    unsigned int getTexture()
    {
        return m_texture;
    }

    Vector2i getSize()
    {
        return m_size;
    }

    Vector2i getBearing()
    {
        return m_bearing;
    }

    unsigned int getAdvance()
    {
        return m_advance;
    }

private:
    unsigned int m_texture;
    Vector2i m_size;
    Vector2i m_bearing;
    unsigned int m_advance;
};