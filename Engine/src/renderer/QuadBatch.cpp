#include <renderer/QuadBatch.h>
#include <util/Timer.h>
#include <renderer/Renderer.h>
#include <renderer/shader/ShaderLibrary.h>

#include <GL/glew.h>

#include <iostream>
#include <cstring>

QuadBatch::QuadBatch()
{
    m_pShader = Renderer::data.textureShader.get();
    create(MAX_QUADS);
}

QuadBatch::QuadBatch(int size, Shader& shader)
{
    m_pShader = &shader;
    create(size);
}

std::shared_ptr<QuadBatch> QuadBatch::createBatch()
{
    return std::make_shared<QuadBatch>();
}

void QuadBatch::create(int size)
{
    if (size > MAX_QUADS)
    {
        Console::err("Too many quads for batch!");
    }

    m_transform = glm::mat4(1.f);
    m_vertexArray.bind();

    BufferLayout layout = {
        { Shader::DataType::Vec2,  "aPos"      },
        { Shader::DataType::Vec2,  "aTexCoord" },
        { Shader::DataType::Color, "aColor"    }
    };

    m_vertexBuffer.setLayout(layout);
    m_vertexBuffer.create(sizeof(Vertex) * Quad::getVertexCount() * size);

    m_vertexArray.addVertexBuffer(m_vertexBuffer);
    m_vertexArray.setIndexBuffer(m_indexBuf);
}

void QuadBatch::start()
{
    m_vertices.clear();
    m_indices.clear();
    m_vertexArray.bind();
}

void QuadBatch::renderQuad(const Texture2D& texture, const Quad& quad)
{
    if (m_pLastTexture == nullptr || texture.getId() != m_pLastTexture->getId())
    {
        swapTexture(texture);
    }

    if (m_vertices.size() / Quad::getVertexCount() > MAX_QUADS)
    {
        Console::err("Too many quads for batch!");
    }

    for (int i = 0 ; i < Quad::indices.size() ; i++)
    {
        m_indices.push_back(Quad::indices[i] + m_vertices.size());
    }

    auto transform = quad.getTransform();
    
    std::array<Vertex, 4> vertices;

    // Populate the vertices array with the quad's vertices
    for (int i = 0 ; i < 4 ; i++)
    {
        auto pos = transform.getMatrix() * glm::vec4(Quad::positions[i].x, Quad::positions[i].y, 0, 1);
        vertices[i].position = Vector2f(pos.x, pos.y);

        // Change "origin" of texCoord, then scale it
        auto rect = quad.getTextureRect();
        
        // Convert Vector2u to Vector2f
        auto texPos = Vector2f(rect.x, rect.y) / Vector2f(texture.getSize().x, texture.getSize().y);
        auto texSize = Vector2f(rect.width, rect.height) / Vector2f(texture.getSize().x, texture.getSize().y);

        vertices[i].texCoord = texPos;
        vertices[i].texCoord += Quad::positions[i] * texSize;

        vertices[i].color = quad.getColor();
    }

    for (auto& vertex : vertices)
    {
        m_vertices.push_back(vertex);
    }
}

void QuadBatch::renderQuad(const Texture2D& texture, const Vector2f& position, const Vector2f& size)
{
    Quad quad(position, size, Color(1, 1, 1, 1));
    quad.setTextureRect(FloatRect(0, 0, texture.getSize().x, texture.getSize().y));
    renderQuad(texture, quad);
}

void QuadBatch::renderQuad(const Texture2D& texture, const Vector2f& position, const Vector2f& size, const FloatRect& texRect)
{
    Quad quad(position, size, Color(1, 1, 1, 1));
    quad.setTextureRect(texRect);
    renderQuad(texture, quad);
}

void QuadBatch::swapTexture(const Texture2D& texture)
{
    flush();
    m_pLastTexture = &texture;
}

void QuadBatch::flush()
{
    if (m_vertices.size() == 0)
    {
        return;
    }

    m_vertexBuffer.update(&m_vertices[0], sizeof(Vertex) * m_vertices.size());
    m_indexBuf.update(&m_indices[0], m_indices.size());

    Renderer::renderArray(m_vertexArray, m_transform, *m_pLastTexture, *m_pShader);
}

void QuadBatch::setTransformMatrix(const glm::mat4& matrix)
{
    m_transform = matrix;
}