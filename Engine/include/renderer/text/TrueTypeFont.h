#pragma once

#include <unordered_map>
#include <memory>

#include <maths/maths/vector/vec2.h>
#include <core/Core.h>
#include <renderer/Texture2D.h>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Glyph
{
    math::vec2 advance;
    math::vec2 size;
    math::vec2 pos;

    float texOffset;
};

class TrueTypeFont
{
public:
    TrueTypeFont();

    void load(const std::string& path, int characterSize);

    static Shared<TrueTypeFont> create(const std::string& path, int characterSize);

    inline const std::unordered_map<char, Glyph>& getGlyphs() const { return m_glyphs; }
    inline const math::vec2& getAtlasSize() const { return m_atlasSize; }
    inline Shared<Texture2D> getTextureAtlas() const { return m_texture; }
    inline int getCharacterSize() const { return m_characterSize; }

private:
    math::vec2 m_atlasSize;

    Shared<Texture2D> m_texture;
    int m_characterSize = 0;

    std::unordered_map<char, Glyph> m_glyphs;
};