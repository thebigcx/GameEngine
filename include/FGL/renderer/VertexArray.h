#pragma once

#include <GL/glew.h>

#include "Buffer.h"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void bind() const;

    void addVertexBuffer(const VertexBuffer& buffer);
    void setIndexBuffer(const IndexBuffer& buffer);

private:
    unsigned int m_id;

    int m_attribCount = 0;
};