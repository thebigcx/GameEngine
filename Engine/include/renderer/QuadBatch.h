#pragma once

#include <vector>
#include <memory>

#include <renderer/Quad.h>
#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>
#include <util/Color.h>
#include <renderer/RenderStates.h>
#include <util/Camera.h>

class QuadBatch
{
public:
    QuadBatch();

    static Shared<QuadBatch> create(int size = MAX_QUADS);
    static Shared<QuadBatch> create(Shader& shader, int size = MAX_QUADS);

    void setTransformMatrix(const glm::mat4& transform);
    void start();
    void renderQuad(const Texture2D& texture, const Quad& quad);
    void renderQuad(const Texture2D& texture, const Vector2f& position, const Vector2f& size);
    void renderQuad(const Texture2D& texture, const Vector2f& position, const Vector2f& size, const FloatRect& texRect);
    void flush();

    void swapTexture(const Texture2D& texture);

    // For Renderer class
    inline const IndexBuffer& getIndexBuffer() const { return m_indexBuf;    }
    inline const VertexArray& getVertexArray() const { return m_vertexArray; }

private:
    VertexArray m_vertexArray;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuf;

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    const Texture2D* m_pLastTexture;
    Shader* m_pShader;

    glm::mat4 m_transform;

    static inline const uint32_t MAX_QUADS = 10000;

    void createBatch(int size);
};