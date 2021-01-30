#pragma once

#include <renderer/VertexArray.h>

namespace Engine
{

class GLVertexArray : public VertexArray
{
public:
    GLVertexArray();
    ~GLVertexArray();

    void bind() const override;
    void unbind() const override;

    void addVertexBuffer(const Reference<VertexBuffer>& buffer) override;
    void setIndexBuffer(const Reference<IndexBuffer>& buffer) override;

    inline Reference<IndexBuffer> getIndexBuffer() const { return m_indexBuffer; }

    inline bool operator==(const VertexArray& array) const override
    {
        return static_cast<const GLVertexArray&>(array).m_id == m_id;
    }

    inline bool operator!=(const VertexArray& array) const override
    {
        return static_cast<const GLVertexArray&>(array).m_id != m_id;
    }

private:
    uint32_t m_id = 0;

    uint32_t m_attribCount = 0;

    Reference<IndexBuffer> m_indexBuffer;
};

}