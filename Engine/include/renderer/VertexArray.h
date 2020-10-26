#pragma once

#include <GL/glew.h>

#include <renderer/Buffer.h>

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void addVertexBuffer(const VertexBuffer& buffer);
    void setIndexBuffer(const IndexBuffer& buffer);

    inline const IndexBuffer* getIndexBuffer() const
    {
        return m_pIndexBuffer;
    }

private:
    unsigned int m_id;

    int m_attribCount = 0;

    const IndexBuffer* m_pIndexBuffer;
};