#include <renderer/text/TextMesh.h>

#include <renderer/shader/Shader.h>
#include <util/math/vector/Vector2.h>
#include <util/math/vector/Vector4.h>
#include <renderer/Renderer.h>

TextMesh::TextMesh()
{

}

void TextMesh::create()
{
    vertexArray.bind();

    BufferLayout layout = {
        { Shader::DataType::Vec2, "aPos" },
        { Shader::DataType::Vec2, "aTexCoord" }
    };

    vertexBuffer.create(sizeof(float) * 1000);
    vertexBuffer.setLayout(layout);
    vertexArray.addVertexBuffer(vertexBuffer);
    vertexArray.setIndexBuffer(indexBuffer);
}