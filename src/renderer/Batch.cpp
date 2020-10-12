#include "FGL/renderer/Batch.h"
#include "FGL/util/Timer.h"

#include <GL/glew.h>

#include <iostream>
#include <cstring>

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
    std::memcpy(indices, quad.m_indices, sizeof(quad.m_indices));

    for (int j = 0 ; j < 6 ; j++)
    {
        indices[j] += m_quads.size() * 4;
    }

    m_indices.insert(m_indices.end(), std::begin(indices), std::end(indices));

    m_quads.push_back(quad);

    m_positionBuf.allocate(sizeof(float) * m_quads.size() * 8);
    m_colorBuf.allocate(sizeof(float) * m_quads.size() * 16);

    m_indexBuf.update(&m_indices[0], m_indices.size());
}

void Batch::update()
{
    m_vertexArray.bind();

    std::vector<Vector2f> positions;
    std::vector<Color> colors;
    
    // TODO: refactor! performance heavy!
    for (auto& quad : m_quads)
    {
        auto i = &quad - &m_quads[0];
        auto vertices = quad.getVertices();

        std::array<Vector2f, 4> vPositions;
        std::array<Color, 4>    vColors;
        for (int j = 0 ; j < vertices.size() ; j++)
        {
            vPositions[j] = vertices[j].position;
            vColors[j] = vertices[j].color;
        }

        positions.insert(positions.end(), vPositions.begin(), vPositions.end());
        colors.insert(colors.end(), vColors.begin(), vColors.end());
    }

    m_positionBuf.update(&positions[0].x, sizeof(float) * positions.size() * 2);
    m_colorBuf.update(&colors[0].r, sizeof(float) * colors.size() * 4);
}

void Batch::render()
{
    {
        update();
    }

    glDrawElements(GL_TRIANGLES, m_indexBuf.getCount(), GL_UNSIGNED_INT, 0);
}