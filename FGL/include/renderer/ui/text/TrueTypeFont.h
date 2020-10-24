#pragma once

#include <array>

#include <util/math/vector/Vector2.h>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Glyph
{
    Vector2f advance;
    Vector2f size;
    Vector2f pos;

    float texOffset;
};

class TrueTypeFont
{
public:
    TrueTypeFont();

    void load(const std::string& path);

private:
    FT_Library m_library;
    FT_Face m_face;

    Vector2f m_atlasSize;

    unsigned int m_texture;

    std::array<Glyph, 256> m_glyphs;
};