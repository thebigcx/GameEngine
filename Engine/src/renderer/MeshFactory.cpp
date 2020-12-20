#include <renderer/MeshFactory.h>
#include <renderer/Renderer2D.h>
#include <renderer/Model.h>
#include <maths/constants.h>

Shared<Mesh> MeshFactory::textMesh()
{
    constexpr uint32_t MAX_CHARACTERS = 200;
    
    auto text = createShared<Mesh>();
    
    text->vertexArray = VertexArray::create();
    text->vertexArray->bind();

    BufferLayout layout = {
        { Shader::DataType::Float2, "aPos" },
        { Shader::DataType::Float2, "aTexCoord" }
    };
    
    uint32_t indices[6 * MAX_CHARACTERS];

    int offset = 0;
    for (unsigned int i = 0; i < MAX_CHARACTERS * 6; i += 6)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;

        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;

        offset += 4;
    }
    text->indexBuffer = IndexBuffer::create(indices, 6 * MAX_CHARACTERS, IndexDataType::UInt32);

    text->vertexBuffer = VertexBuffer::create(sizeof(float) * 4 * MAX_CHARACTERS);
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
        { Shader::DataType::Float2, "aPos" },
        { Shader::DataType::Float2, "aTexCoord" }
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

    quad->indexBuffer = IndexBuffer::create(6, IndexDataType::UInt32);
    quad->indexBuffer->setData(indices, 6);

    quad->vertexBuffer = VertexBuffer::create(sizeof(float) * 4 * 4);
    quad->vertexBuffer->setData(vertices, 16 * sizeof(float));
    quad->vertexBuffer->setLayout(layout);
    quad->vertexArray->addVertexBuffer(quad->vertexBuffer);
    quad->vertexArray->setIndexBuffer(quad->indexBuffer);

    return quad;
}

Shared<Mesh> MeshFactory::cubeMesh(float size, const Shared<Material>& material)
{
    auto quad = createShared<Mesh>();

    quad->vertexArray = VertexArray::create();
    quad->vertexArray->bind();

    BufferLayout layout = {
        { Shader::DataType::Float3, "aPos" },
        { Shader::DataType::Float3, "aNormal" },
        { Shader::DataType::Float2, "aTexCoord" }
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

    quad->indexBuffer = IndexBuffer::create(36, IndexDataType::UInt32);
    quad->indexBuffer->setData(indices, 36);

    quad->vertexBuffer = VertexBuffer::create(sizeof(ModelVertex) * 24);
    quad->vertexBuffer->setData(vertices, sizeof(ModelVertex) * 24);
    quad->vertexBuffer->setLayout(layout);
    quad->vertexArray->addVertexBuffer(quad->vertexBuffer);
    quad->vertexArray->setIndexBuffer(quad->indexBuffer);

    quad->materials.push_back(material);

    return quad;
}

Shared<Mesh> MeshFactory::skyboxMesh()
{
    auto box = createShared<Mesh>();

    box->vertexArray = VertexArray::create();
    box->vertexArray->bind();

    BufferLayout layout = {
        { Shader::DataType::Float3, "aPos" }
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

    box->indexBuffer = IndexBuffer::create(36, IndexDataType::UInt32);
    box->indexBuffer->setData(indices, 36);

    box->vertexBuffer = VertexBuffer::create(sizeof(math::vec3) * 24);
    box->vertexBuffer->setData(vertices, sizeof(math::vec3) * 24);
    box->vertexBuffer->setLayout(layout);
    box->vertexArray->addVertexBuffer(box->vertexBuffer);
    box->vertexArray->setIndexBuffer(box->indexBuffer);

    return box;
}

Shared<Mesh> MeshFactory::sphereMesh(float radius, int sectors, int stacks)
{
    std::vector<uint32_t> indices;
    std::vector<ModelVertex> vertices;

    float PI = math::pi<float>();

    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.f / radius;
    float u, v;

    float sectorStep = 2 * PI / sectors;
    float stackStep = PI / stacks;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stacks; ++i)
    {
        stackAngle = PI / 2.f - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j)
        { // TODO: change loop indices (++j)
            ModelVertex vertex;

            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            vertex.position = { x, y, z };

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            vertex.normal = { nx, ny, nz };

            u = static_cast<float>(j) / sectors;
            v = static_cast<float>(i) / stacks;

            vertex.uv = { u, v };

            vertices.push_back(vertex);
        }
    }

    // Indices
    int k1, k2;
    for (int i = 0; i < stacks; ++i)
    {
        k1 = i * (sectors + 1);
        k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);

                math::vec3 tangent = calculateTangent(vertices[k1].position, 
                                                      vertices[k2].position, 
                                                      vertices[k1 + 1].position,
                                                      vertices[k1].uv, 
                                                      vertices[k2].uv, 
                                                      vertices[k1 + 1].uv);
                
                vertices[k1].tangent = tangent;
                vertices[k2].tangent = tangent;
                vertices[k1 + 1].tangent = tangent;
            }

            if (i != (stacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);

                math::vec3 tangent = calculateTangent(vertices[k1 + 1].position, 
                                                      vertices[k2].position, 
                                                      vertices[k2 + 1].position,
                                                      vertices[k1 + 1].uv, 
                                                      vertices[k2].uv, 
                                                      vertices[k2 + 1].uv);
                
                vertices[k1 + 1].tangent = tangent;
                vertices[k2].tangent = tangent;
                vertices[k2 + 1].tangent = tangent;
            }
        }
    }

    auto sphere = createShared<Mesh>();

    sphere->vertexArray = VertexArray::create();
    sphere->vertexArray->bind();

    BufferLayout layout = {
        { Shader::DataType::Float3, "aPos"      },
        { Shader::DataType::Float3, "aNormal"   },
        { Shader::DataType::Float2, "aTexCoord" },
        { Shader::DataType::Float3, "aTangent"  }
    };

    sphere->indexBuffer = IndexBuffer::create(indices.size(), IndexDataType::UInt32);
    sphere->indexBuffer->setData(&indices[0], indices.size());

    sphere->vertexBuffer = VertexBuffer::create(sizeof(ModelVertex) * vertices.size());
    sphere->vertexBuffer->setData(&vertices[0], sizeof(ModelVertex) * vertices.size());
    sphere->vertexBuffer->setLayout(layout);
    sphere->vertexArray->addVertexBuffer(sphere->vertexBuffer);
    sphere->vertexArray->setIndexBuffer(sphere->indexBuffer);

    return sphere;
}

math::vec3 MeshFactory::calculateTangent(
    const math::vec3& pos1,
    const math::vec3& pos2, 
    const math::vec3& pos3,
    const math::vec2& uv1,
    const math::vec2& uv2,
    const math::vec2& uv3)
{
    math::vec3 edge1 = pos2 - pos1;
    math::vec3 edge2 = pos3 - pos1;
    math::vec2 deltaUV1 = uv2 - uv1;
    math::vec2 deltaUV2 = uv3 - uv1;

    float f = 1.f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    math::vec3 tangent;
    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    return tangent;
}