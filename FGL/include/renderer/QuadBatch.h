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
    QuadBatch(Texture& texture);

    void create(Texture& texture);

    void start();
    void renderQuad(const Quad& quad);
    void flush();

    // For Renderer class
    inline const IndexBuffer&  getIndexBuffer() const { return m_indexBuf;    }
    inline const VertexArray&  getVertexArray() const { return m_vertexArray; }
    inline const Texture* getTexture()          const { return m_pTexture;    }

private:
    VertexArray m_vertexArray;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuf;

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    Texture* m_pTexture;

    static inline const unsigned int MAX_QUADS = 10000;
};