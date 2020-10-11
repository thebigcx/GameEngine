#include "Batch.h"

#include <GL/glew.h>

#include <iostream>

Batch::Batch()
{
    m_vertexArray.bind();

    m_positionBuf.bind();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
}

void Batch::add(Quad& quad)
{
    m_positions.insert(m_positions.end(), std::begin(quad.m_positions), std::end(quad.m_positions));
    m_indices.insert(m_indices.end(), std::begin(quad.m_indices), std::end(quad.m_indices));

    m_vertexArray.bind();

    m_positionBuf.update(&m_positions[0], sizeof(float) * m_positions.size());
    m_indexBuf.update(&m_indices[0], m_indices.size());
}

void Batch::render()
{
    m_vertexArray.bind();

    glDrawElements(GL_TRIANGLES, m_indexBuf.getCount(), GL_UNSIGNED_INT, 0);
}