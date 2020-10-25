#include <algorithm>

#include <GL/glew.h>

#include <renderer/ui/text/TrueTypeFont.h>
#include <core/Console.h>

TrueTypeFont::TrueTypeFont()
{
    
}

void TrueTypeFont::load(const std::string& path)
{
    if (FT_Init_FreeType(&m_library))
    {
        Console::err("Could not initialise FreeType.");
        return;
    }

    if (FT_New_Face(m_library, path.c_str(), 0, &m_face))
    {
        Console::err("Could not create font face (check file path).");
        return;
    }

    FT_Set_Pixel_Sizes(m_face, 0, 48);  

    FT_GlyphSlot g = m_face->glyph;
    int w = 0;
    int h = 0;

    for (int i = 32; i < 255; i++)
    {
        if (FT_Load_Char(m_face, i, FT_LOAD_RENDER))
        {
            Console::err("Could not load character: " + static_cast<char>(i));
        }

        w += g->bitmap.width;
        h = std::max(h, static_cast<int>(g->bitmap.rows));
    }

    m_atlasSize.x = w;
    m_atlasSize.y = h;

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_atlasSize.x, m_atlasSize.y, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    int x = 0;
    for (int i = 32; i < 255; i++)
    {
        if (FT_Load_Char(m_face, i, FT_LOAD_RENDER))
        {
            continue;
        }

        glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

        m_glyphs[i].advance.x = g->advance.x >> 6;
        m_glyphs[i].advance.y = g->advance.y >> 6;

        m_glyphs[i].size.x = g->bitmap.width;
        m_glyphs[i].size.y = g->bitmap.rows;

        m_glyphs[i].pos.x = g->bitmap_left;
        m_glyphs[i].pos.y = g->bitmap_top;

        m_glyphs[i].texOffset = static_cast<float>(x) / m_atlasSize.x;

        x += g->bitmap.width;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}