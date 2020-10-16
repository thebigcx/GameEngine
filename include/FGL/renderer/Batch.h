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

    void add(const Quad& quad);

    void update();

    // For Renderer class
    const IndexBuffer& getIndexBuffer() const
    {
        return m_indexBuf;
    }

    const VertexArray& getVertexArray() const
    {
        return m_vertexArray;
    }

private:
    VertexArray m_vertexArray;

    VertexBuffer m_vertexBuffer;

    IndexBuffer m_indexBuf;

    std::vector<Vector2f> m_positions;
    std::vector<Color>    m_colors;

    std::vector<unsigned int> m_indices;

    std::vector<const Quad*> m_quads;
};