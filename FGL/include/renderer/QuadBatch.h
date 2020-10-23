#pragma once

#include <vector>

#include <renderer/Quad.h>
#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>
#include <util/Color.h>
#include <renderer/RenderStates.h>
#include <util/Camera.h>

class QuadBatch
{
public:
    QuadBatch() {}
    QuadBatch(Texture2D& texture);

    void create(Texture2D& texture);

    void setTransformMatrix(const glm::mat4& transform);
    void start();
    void renderQuad(const Quad& quad);
    void flush();

    // For Renderer class
    inline const IndexBuffer&  getIndexBuffer() const { return m_indexBuf;    }
    inline const VertexArray&  getVertexArray() const { return m_vertexArray; }
    inline const Texture2D*    getTexture()     const { return m_pTexture;    }

private:
    VertexArray m_vertexArray;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuf;

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    Texture2D* m_pTexture;

    glm::mat4 m_transform;

    static inline const unsigned int MAX_QUADS = 10000;
};