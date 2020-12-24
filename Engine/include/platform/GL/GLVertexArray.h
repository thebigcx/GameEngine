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

    void addVertexBuffer(const Shared<VertexBuffer>& buffer) override;
    void setIndexBuffer(const Shared<IndexBuffer>& buffer) override;

    inline Shared<IndexBuffer> getIndexBuffer() const { return m_indexBuffer; }

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

    Shared<IndexBuffer> m_indexBuffer;
};

}