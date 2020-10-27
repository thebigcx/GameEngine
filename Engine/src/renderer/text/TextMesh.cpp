#include <renderer/text/TextMesh.h>

#include <renderer/Shader.h>
#include <util/math/vector/Vector2.h>
#include <util/math/vector/Vector4.h>
#include <renderer/Renderer.h>

Shader TextMesh::shader;

TextMesh::TextMesh()
{

}

void TextMesh::create()
{
    shader.create("shaders/text.vert", "shaders/text.frag");
    shader.bind();
    shader.setUniform("projection", Renderer::data.projectionMatrix);

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