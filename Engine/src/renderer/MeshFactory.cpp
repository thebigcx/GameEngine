#include <renderer/MeshFactory.h>

Shared<Mesh> MeshFactory::textMesh()
{
    auto text = createShared<Mesh>();
    
    text->vertexArray.bind();

    BufferLayout layout = {
        { Shader::DataType::Vec2, "aPos" },
        { Shader::DataType::Vec2, "aTexCoord" }
    };

    text->vertexBuffer.create(sizeof(float) * 4 * 200);
    text->vertexBuffer.setLayout(layout);
    text->vertexArray.addVertexBuffer(text->vertexBuffer);
    text->vertexArray.setIndexBuffer(text->indexBuffer);

    return text;
}