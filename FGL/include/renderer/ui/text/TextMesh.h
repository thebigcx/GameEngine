#pragma once

#include <string>

#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>
#include <renderer/ui/text/TrueTypeFont.h>

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
    void renderText(const std::string& text, TrueTypeFont& font);

private:
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
    VertexArray m_vertexArray;

    static Shader m_shader;
};