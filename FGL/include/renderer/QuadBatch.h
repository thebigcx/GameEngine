#pragma once

#include <vector>

#include <renderer/Quad.h>
#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>
#include <util/Color.h>
#include <renderer/RenderStates.h>

class QuadBatch
{
public:
    QuadBatch() {}
    QuadBatch(RenderStates states);

    void create(RenderStates states);

    void start();
    void renderQuad(const Quad& quad);
    void flush();

    // For Renderer class
    inline const IndexBuffer&  getIndexBuffer() const { return m_indexBuf;    }
    inline const VertexArray&  getVertexArray() const { return m_vertexArray; }
    inline const RenderStates& getStates()      const { return m_states;      }

private:
    VertexArray m_vertexArray;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuf;

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    RenderStates m_states;

    static inline const unsigned int MAX_QUADS = 10000;
};