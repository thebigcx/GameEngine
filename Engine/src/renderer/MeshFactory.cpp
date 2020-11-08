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

Shared<Mesh> MeshFactory::quadMesh(float x1, float y1, float x2, float y2)
{
    auto quad = createShared<Mesh>();

    quad->vertexArray.bind();

    BufferLayout layout = {
        { Shader::DataType::Vec2, "aPos" },
        { Shader::DataType::Vec2, "aTexCoord" }
    };

    uint32_t indices[] = {
        0, 1, 2, 2, 3, 0
    };

    float vertices[] = {
        x1, y1, 0, 0,
        x1, y2, 0, 1,
        x2, y2, 1, 1,
        x2, y1, 1, 0
    };

    quad->indexBuffer.update(indices, 6);

    quad->vertexBuffer.create(sizeof(float) * 4 * 4);
    quad->vertexBuffer.update(vertices, 16 * sizeof(float));
    quad->vertexBuffer.setLayout(layout);
    quad->vertexArray.addVertexBuffer(quad->vertexBuffer);
    quad->vertexArray.setIndexBuffer(quad->indexBuffer);

    return quad;
}