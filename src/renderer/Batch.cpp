#include "renderer/Batch.h"

#include <GL/glew.h>

#include <iostream>

Batch::Batch()
{
    m_vertexArray.bind();

    m_positionBuf.bind();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    m_colorBuf.bind();
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
}

void Batch::add(Quad& quad)
{
    // Add an offset to the indices
    int indices[6];
    for (int i = 0 ; i < 6 ; i++)
    {
        indices[i] = quad.m_indices[i] + (m_indices.size() / 6) * 4;
    }

    m_positions.insert(m_positions.end(), std::begin(quad.m_positions), std::end(quad.m_positions));
    m_colors.insert(m_colors.end(), std::begin(quad.m_colors), std::end(quad.m_colors));
    m_indices.insert(m_indices.end(), std::begin(indices), std::end(indices));

    m_vertexArray.bind();

    m_positionBuf.update(&m_positions[0].x, sizeof(float) * m_positions.size() * 2);
    m_colorBuf.update(&m_colors[0].r, sizeof(float) * m_colors.size() * 4);
    m_indexBuf.update(&m_indices[0], m_indices.size());
}

void Batch::render()
{
    m_vertexArray.bind();

    glDrawElements(GL_TRIANGLES, m_indexBuf.getCount(), GL_UNSIGNED_INT, 0);
}