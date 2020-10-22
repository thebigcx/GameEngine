#include <renderer/QuadBatch.h>
#include <util/Timer.h>
#include <renderer/Renderer.h>
#include <renderer/ShaderLibrary.h>

#include <GL/glew.h>

#include <iostream>
#include <cstring>

QuadBatch::QuadBatch(Texture2D& texture)
{
    create(texture);
}

void QuadBatch::create(Texture2D& texture)
{
    m_pTexture = &texture;
    m_vertexArray.bind();

    BufferLayout layout = {
        { Shader::DataType::Vec2, "aPos" },
        { Shader::DataType::Vec2, "aTexCoord" },
        { Shader::DataType::Color, "aColor" }
    };

    m_vertexBuffer.setLayout(layout);
    m_vertexBuffer.create(sizeof(Vertex) * Quad::getVertexCount() * MAX_QUADS);

    m_vertexArray.addVertexBuffer(m_vertexBuffer);
    m_vertexArray.setIndexBuffer(m_indexBuf);
}

void QuadBatch::start()
{
    m_vertices.clear();
    m_indices.clear();
    m_vertexArray.bind();
}

void QuadBatch::renderQuad(const Quad& quad)
{
    if (m_vertices.size() / Quad::getVertexCount() > MAX_QUADS)
    {
        Logger::err("Too many quads for batch!");
    }

    for (int i = 0 ; i < quad.getIndices().size() ; i++)
    {
        m_indices.push_back(quad.getIndices()[i] + m_vertices.size());
    }

    for (auto vertex : quad.getVertices())
    {
        m_vertices.push_back(vertex);
    }
}

void QuadBatch::flush()
{
    m_vertexBuffer.update(&m_vertices[0], sizeof(Vertex) * m_vertices.size());
    m_indexBuf.update(&m_indices[0], m_indices.size());

    Renderer::render(m_vertexArray, glm::mat4(1.f), *m_pTexture);
}