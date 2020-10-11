#pragma once

#include <vector>

#include "Quad.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

class Batch
{
public:
    Batch();

    void add(Quad& quad);

    void render();

private:
    //unsigned int m_vao;

    VertexArray m_vertexArray;
    VertexBuffer m_positionBuf;
    IndexBuffer m_indexBuf;

    std::vector<float> m_positions;
    std::vector<unsigned int> m_indices;
};