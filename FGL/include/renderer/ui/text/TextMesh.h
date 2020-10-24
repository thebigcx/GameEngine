#pragma once

#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>
#include <renderer/ui/text/UITextComponent.h>

class TextMesh
{
public:
    TextMesh();

    void create();
    void renderText(UITextComponent& text);

private:
    VertexArray m_vertexArray;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;

    Shader* m_pShader;
};