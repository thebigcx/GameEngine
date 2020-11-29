#include <renderer/MeshFactory.h>
#include <renderer/Vertex.h>
#include <renderer/Model.h>

Shared<Mesh> MeshFactory::textMesh()
{
    auto text = createShared<Mesh>();
    
    text->vertexArray = VertexArray::create();
    text->vertexArray->bind();

    BufferLayout layout = {
        { Shader::DataType::Vec2, "aPos" },
        { Shader::DataType::Vec2, "aTexCoord" }
    };
    text->indexBuffer = IndexBuffer::create(6 * 200);

    text->vertexBuffer = VertexBuffer::create(sizeof(float) * 4 * 200);
    text->vertexBuffer->setLayout(layout);
    text->vertexArray->addVertexBuffer(text->vertexBuffer);
    text->vertexArray->setIndexBuffer(text->indexBuffer);

    return text;
}

Shared<Mesh> MeshFactory::quadMesh(float x1, float y1, float x2, float y2)
{
    auto quad = createShared<Mesh>();

    quad->vertexArray = VertexArray::create();
    quad->vertexArray->bind();

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

    quad->indexBuffer = IndexBuffer::create(6);
    quad->indexBuffer->setData(indices, 6);

    quad->vertexBuffer = VertexBuffer::create(sizeof(float) * 4 * 4);
    quad->vertexBuffer->setData(vertices, 16 * sizeof(float));
    quad->vertexBuffer->setLayout(layout);
    quad->vertexArray->addVertexBuffer(quad->vertexBuffer);
    quad->vertexArray->setIndexBuffer(quad->indexBuffer);

    return quad;
}

Shared<Mesh> MeshFactory::cubeMesh(float size)
{
    auto quad = createShared<Mesh>();

    quad->vertexArray = VertexArray::create();
    quad->vertexArray->bind();

    BufferLayout layout = {
        { Shader::DataType::Vec3, "aPos" },
        { Shader::DataType::Vec3, "aNormal" },
        { Shader::DataType::Vec2, "aTexCoord" }
    };

    uint32_t indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    float hSize = size / 2.f;

    ModelVertex vertices[] = {
        {math::vec3(-hSize, -hSize,  hSize), math::vec3( 0,  0,  1), math::vec2(0, 0)}, // Front
        {math::vec3(-hSize,  hSize,  hSize), math::vec3( 0,  0,  1), math::vec2(0, 1)},
        {math::vec3( hSize,  hSize,  hSize), math::vec3( 0,  0,  1), math::vec2(1, 1)},
        {math::vec3( hSize, -hSize,  hSize), math::vec3( 0,  0,  1), math::vec2(1, 0)},

        {math::vec3(-hSize, -hSize, -hSize), math::vec3( 0,  0, -1), math::vec2(0, 0)}, // Back
        {math::vec3(-hSize,  hSize, -hSize), math::vec3( 0,  0, -1), math::vec2(0, 1)},
        {math::vec3( hSize,  hSize, -hSize), math::vec3( 0,  0, -1), math::vec2(1, 1)},
        {math::vec3( hSize, -hSize, -hSize), math::vec3( 0,  0, -1), math::vec2(1, 0)},

        {math::vec3(-hSize,  hSize, -hSize), math::vec3( 0,  1,  0), math::vec2(0, 0)}, // Top
        {math::vec3(-hSize,  hSize,  hSize), math::vec3( 0,  1,  0), math::vec2(0, 1)},
        {math::vec3( hSize,  hSize,  hSize), math::vec3( 0,  1,  0), math::vec2(1, 1)},
        {math::vec3( hSize,  hSize, -hSize), math::vec3( 0,  1,  0), math::vec2(1, 0)},

        {math::vec3(-hSize, -hSize, -hSize), math::vec3( 0, -1,  0), math::vec2(0, 0)}, // Bottom
        {math::vec3(-hSize, -hSize,  hSize), math::vec3( 0, -1,  0), math::vec2(0, 1)},
        {math::vec3( hSize, -hSize,  hSize), math::vec3( 0, -1,  0), math::vec2(1, 1)},
        {math::vec3( hSize, -hSize, -hSize), math::vec3( 0, -1,  0), math::vec2(1, 0)},

        {math::vec3(-hSize, -hSize, -hSize), math::vec3(-1,  0,  0), math::vec2(0, 0)}, // Left
        {math::vec3(-hSize,  hSize, -hSize), math::vec3(-1,  0,  0), math::vec2(0, 1)},
        {math::vec3(-hSize,  hSize,  hSize), math::vec3(-1,  0,  0), math::vec2(1, 1)},
        {math::vec3(-hSize, -hSize,  hSize), math::vec3(-1,  0,  0), math::vec2(1, 0)},

        {math::vec3( hSize, -hSize, -hSize), math::vec3( 1,  0,  0), math::vec2(0, 0)}, // Right
        {math::vec3( hSize,  hSize, -hSize), math::vec3( 1,  0,  0), math::vec2(0, 1)},
        {math::vec3( hSize,  hSize,  hSize), math::vec3( 1,  0,  0), math::vec2(1, 1)},
        {math::vec3( hSize, -hSize,  hSize), math::vec3( 1,  0,  0), math::vec2(1, 0)}
    };

    quad->indexBuffer = IndexBuffer::create(36);
    quad->indexBuffer->setData(indices, 36);

    quad->vertexBuffer = VertexBuffer::create(sizeof(ModelVertex) * 24);
    quad->vertexBuffer->setData(vertices, sizeof(ModelVertex) * 24);
    quad->vertexBuffer->setLayout(layout);
    quad->vertexArray->addVertexBuffer(quad->vertexBuffer);
    quad->vertexArray->setIndexBuffer(quad->indexBuffer);

    return quad;
}

Shared<Mesh> MeshFactory::skyboxMesh()
{
    auto box = createShared<Mesh>();

    box->vertexArray = VertexArray::create();
    box->vertexArray->bind();

    BufferLayout layout = {
        { Shader::DataType::Vec3, "aPos" }
    };

    uint32_t indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    math::vec3 vertices[] = {
        math::vec3(-1.f, -1.f,  1.f), // Front
        math::vec3(-1.f,  1.f,  1.f),
        math::vec3( 1.f,  1.f,  1.f),
        math::vec3( 1.f, -1.f,  1.f),

        math::vec3(-1.f, -1.f, -1.f), // Back
        math::vec3(-1.f,  1.f, -1.f),
        math::vec3( 1.f,  1.f, -1.f),
        math::vec3( 1.f, -1.f, -1.f),

        math::vec3(-1.f,  1.f, -1.f), // Top
        math::vec3(-1.f,  1.f,  1.f),
        math::vec3( 1.f,  1.f,  1.f),
        math::vec3( 1.f,  1.f, -1.f),

        math::vec3(-1.f, -1.f, -1.f), // Bottom
        math::vec3(-1.f, -1.f,  1.f),
        math::vec3( 1.f, -1.f,  1.f),
        math::vec3( 1.f, -1.f, -1.f),

        math::vec3(-1.f, -1.f, -1.f), // Left
        math::vec3(-1.f,  1.f, -1.f),
        math::vec3(-1.f,  1.f,  1.f),
        math::vec3(-1.f, -1.f,  1.f),

        math::vec3( 1.f, -1.f, -1.f), // Right
        math::vec3( 1.f,  1.f, -1.f),
        math::vec3( 1.f,  1.f,  1.f),
        math::vec3( 1.f, -1.f,  1.f)
    };

    box->indexBuffer = IndexBuffer::create(36);
    box->indexBuffer->setData(indices, 36);

    box->vertexBuffer = VertexBuffer::create(sizeof(math::vec3) * 24);
    box->vertexBuffer->setData(vertices, sizeof(math::vec3) * 24);
    box->vertexBuffer->setLayout(layout);
    box->vertexArray->addVertexBuffer(box->vertexBuffer);
    box->vertexArray->setIndexBuffer(box->indexBuffer);

    return box;
}