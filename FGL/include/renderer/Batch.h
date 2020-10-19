#pragma once

#include <vector>

#include <renderer/Quad.h>
#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>
#include <util/Color.h>
#include <renderer/RenderStates.h>

class Batch
{
public:
    Batch() {}

    Batch(RenderStates states);

    void create(RenderStates states);

    void add(const Quad& quad);

    void update();

    // For Renderer class
    inline const IndexBuffer& getIndexBuffer() const
    {
        return m_indexBuf;
    }

    inline const VertexArray& getVertexArray() const
    {
        return m_vertexArray;
    }

    inline RenderStates getStates() const
    {
        return m_states;
    }

private:
    VertexArray m_vertexArray;

    VertexBuffer m_vertexBuffer;

    IndexBuffer m_indexBuf;

    std::vector<Vector2f> m_positions;
    std::vector<Color>    m_colors;

    std::vector<unsigned int> m_indices;

    std::vector<const Quad*> m_quads;

    RenderStates m_states;
};