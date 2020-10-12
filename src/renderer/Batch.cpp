#include "FGL/renderer/Batch.h"
#include "FGL/util/Timer.h"

#include <GL/glew.h>

#include <iostream>
#include <cstring>

Batch::Batch()
{
    m_vertexArray.bind();

    m_vertexBuffer.bind();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Batch::add(const Quad& quad)
{
    // Add an offset to the indices
    int indices[quad.m_indices.size()];
    std::memcpy(indices, quad.m_indices.data(), sizeof(quad.m_indices));

    for (int j = 0 ; j < quad.m_indices.size() ; j++)
    {
        indices[j] += m_quads.size() * Quad::getVertexCount();
    }

    m_indices.insert(m_indices.end(), std::begin(indices), std::end(indices));

    m_quads.push_back(&quad);

    m_vertexBuffer.allocate(sizeof(Vertex) * m_quads.size() * Quad::getVertexCount());

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

void Batch::render()
{
    {
        update();
    }
        
    glDrawElements(GL_TRIANGLES, m_indexBuf.getCount(), GL_UNSIGNED_INT, 0);
}