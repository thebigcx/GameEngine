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

    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    VertexArray vertexArray;

    static Shader shader;

private:
    
};