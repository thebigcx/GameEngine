#pragma once

#include <renderer/VertexArray.h>

class GLVertexArray : public VertexArray
{
public:
    GLVertexArray();
    ~GLVertexArray();

    void bind() const override;
    void unbind() const override;

    void addVertexBuffer(Shared<VertexBuffer> buffer) override;
    void setIndexBuffer(Shared<IndexBuffer> buffer) override;

    Shared<IndexBuffer> getIndexBuffer() const;

private:
    unsigned int m_id = 0;

    int m_attribCount = 0;

    Shared<IndexBuffer> m_pIndexBuffer;
};