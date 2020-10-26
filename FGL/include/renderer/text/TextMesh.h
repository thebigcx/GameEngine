#pragma once

#include <string>

#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>
#include <renderer/text/TrueTypeFont.h>
#include <renderer/text/Text.h>

struct GlyphVertex
{
    Vector2f pos;
    Vector2f texCoord;
};

class TextMesh
{
public:
    TextMesh();

    void create();
    void renderText(const std::string& text, const TrueTypeFont& font, const Color& color, const Vector2f& size);
    void renderText(const Text& text);

private:
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
    VertexArray m_vertexArray;

    static Shader m_shader;
};