#include <algorithm>

#include <GL/glew.h>

#include <renderer/text/TrueTypeFont.h>
#include <core/Logger.h>

TrueTypeFont::TrueTypeFont()
{
    
}

Shared<TrueTypeFont> TrueTypeFont::create(const std::string& path, int characterSize)
{
    auto ptr = createShared<TrueTypeFont>();
    ptr->load(path, characterSize);
    return ptr;
}

void TrueTypeFont::load(const std::string& path, int characterSize)
{
    m_characterSize = characterSize;

    FT_Library library;
    FT_Face face;

    if (FT_Init_FreeType(&library))
    {
        Logger::getCoreLogger()->error("Could not initialize FreeType.");
        return;
    }

    if (FT_New_Face(library, path.c_str(), 0, &face))
    {
        Logger::getCoreLogger()->error("Could not load font: %s", path);
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, characterSize);

    FT_GlyphSlot g = face->glyph;
    int w = 0;
    int h = 0;

    for (unsigned int i = 32; i < 255; i++)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
        {
            Logger::getCoreLogger()->error("Could not load character: %s" + static_cast<char>(i));
        }

        w += g->bitmap.width;
        h = std::max(h, static_cast<int>(g->bitmap.rows));
    }

    m_atlasSize.x = w;
    m_atlasSize.y = h;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    m_texture = Texture2D::create(m_atlasSize.x, m_atlasSize.y);

    int x = 0;
    for (unsigned int i = 32; i < 255; i++)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
        {
            continue;
        }

        m_texture->setData(x, 0, g->bitmap.width, g->bitmap.rows, g->bitmap.buffer, GL_RED);

        m_glyphs[i].advance.x = g->advance.x >> 6;
        m_glyphs[i].advance.y = g->advance.y >> 6;

        m_glyphs[i].size.x = g->bitmap.width;
        m_glyphs[i].size.y = g->bitmap.rows;

        m_glyphs[i].pos.x = g->bitmap_left;
        m_glyphs[i].pos.y = g->bitmap_top;

        m_glyphs[i].texOffset = static_cast<float>(x) / m_atlasSize.x;

        x += g->bitmap.width;
    }

    m_texture->setParameter(Texture2D::Parameter::WrapS, Texture2D::Value::ClampToEdge);
    m_texture->setParameter(Texture2D::Parameter::WrapT, Texture2D::Value::ClampToEdge);

    m_texture->setParameter(Texture2D::Parameter::MinFilter, Texture2D::Value::Nearest);
    m_texture->setParameter(Texture2D::Parameter::MagFilter, Texture2D::Value::Nearest);

    FT_Done_Face(face);
    FT_Done_FreeType(library);
}