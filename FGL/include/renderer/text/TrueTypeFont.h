#pragma once

#include <unordered_map>
#include <memory>

#include <util/math/vector/Vector2.h>
#include <core/Core.h>

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

    static Shared<TrueTypeFont> create(const std::string& path);

    inline const std::unordered_map<char, Glyph>& getGlyphs() const { return m_glyphs; }
    inline const Vector2f& getAtlasSize() const { return m_atlasSize; }
    inline unsigned int getTextureAtlas() const { return m_texture; }

    static inline const unsigned int FONT_SIZE = 48;

private:
    Vector2f m_atlasSize;

    unsigned int m_texture;

    std::unordered_map<char, Glyph> m_glyphs;
};