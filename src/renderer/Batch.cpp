#include "FGL/renderer/Batch.h"
#include "FGL/util/Timer.h"

#include <GL/glew.h>

#include <iostream>
#include <cstring>

Batch::Batch(const Shader& shader)
{
    create(shader);
}

void Batch::create(const Shader& shader)
{
    m_pShader = &shader;
    m_vertexArray.bind();

    BufferLayout layout = {
        { Shader::DataType::Vec2f, "aPos" },
        { Shader::DataType::Vec2f, "aTexCoord" },
        { Shader::DataType::Color, "aColor" }
    };

    m_vertexBuffer.setLayout(layout);

    m_vertexArray.addVertexBuffer(m_vertexBuffer);
    m_vertexArray.setIndexBuffer(m_indexBuf);
}

void Batch::add(const Quad& quad)
{
    // Add an offset to the indices
    int indices[quad.getIndices().size()];
    std::memcpy(indices, quad.getIndices().data(), sizeof(quad.getIndices()));

    for (int j = 0 ; j < quad.getIndices().size() ; j++)
    {
        indices[j] += m_quads.size() * Quad::getVertexCount();
    }

    m_indices.insert(m_indices.end(), &indices[0], &indices[sizeof(indices)]);

    m_quads.push_back(&quad);

    m_vertexBuffer.create(sizeof(Vertex) * m_quads.size() * Quad::getVertexCount());

    m_indexBuf.update(&m_indices[0], m_indices.size());
}

void Batch::update()
{
    m_vertexArray.bind();

    std::vector<Vertex> vertices;
    
    for (auto quad : m_quads)
    {
        auto i = &quad - &m_quads[0];
        auto qVertices = quad->getVertices();

        vertices.insert(vertices.end(), qVertices.begin(), qVertices.end());
    }

    m_vertexBuffer.update(&vertices[0], sizeof(Vertex) * vertices.size());
}