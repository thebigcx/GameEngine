#pragma once

#include <vector>

#include "Quad.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "../util/Color.h"

class Batch
{
public:
    Batch();

    void add(Quad& quad);

    void render();

private:
    VertexArray m_vertexArray;

    VertexBuffer m_positionBuf;
    VertexBuffer m_colorBuf;

    IndexBuffer m_indexBuf;

    std::vector<Vector2f> m_positions;
    std::vector<Color>    m_colors;

    std::vector<unsigned int> m_indices;
};