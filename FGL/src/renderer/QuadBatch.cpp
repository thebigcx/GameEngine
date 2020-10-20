#include <renderer/QuadBatch.h>
#include <util/Timer.h>
#include <renderer/Renderer.h>

#include <GL/glew.h>

#include <iostream>
#include <cstring>

QuadBatch::QuadBatch(RenderStates states)
{
    create(states);
}

void QuadBatch::create(RenderStates states)
{
    m_states = states;
    m_vertexArray.bind();

    BufferLayout layout = {
        { Shader::DataType::Vec2f, "aPos" },
        { Shader::DataType::Vec2f, "aTexCoord" },
        { Shader::DataType::Color, "aColor" }
    };

    m_vertexBuffer.setLayout(layout);
    m_vertexBuffer.create(sizeof(Vertex) * Quad::getVertexCount() * MAX_QUADS);

    m_vertexArray.addVertexBuffer(m_vertexBuffer);
    m_vertexArray.setIndexBuffer(m_indexBuf);
}

void QuadBatch::begin()
{
    m_vertices.clear();
    m_indices.clear();
    m_vertexArray.bind();
}

void QuadBatch::draw(const Quad& quad)
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

    Renderer::renderIndexed(m_vertexArray, m_states);
}