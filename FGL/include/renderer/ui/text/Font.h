#pragma once

#include <GL/glew.h>

#include <iostream>
#include <string>
#include <unordered_map>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <renderer/ui/text/Glyph.h>

class Font
{
public:
    Font();

    bool loadFile(const std::string& path);

    std::unordered_map<char, Glyph>& getCharacters()
    {
        return m_characters;
    }

    int getSize() const
    {
        return m_size;
    }

private:
    FT_Library m_ft;
    FT_Face m_face;

    std::unordered_map<char, Glyph> m_characters;

    Glyph loadGlyph(char character) const;

    int m_size = 0;
};