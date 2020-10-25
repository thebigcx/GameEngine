#pragma once

#include <string>

#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>
#include <renderer/ui/text/TrueTypeFont.h>

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