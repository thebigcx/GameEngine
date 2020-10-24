#pragma once

#include <string>

#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>

class TextMesh
{
public:
    TextMesh();

    void create();
    void renderText(const std::string& text);

private:
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
    VertexArray m_vertexArray;
};