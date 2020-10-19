#include "renderer/VertexArray.h"
#include "renderer/Vertex.h"

#include "core/Logger.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bind() const
{
    glBindVertexArray(m_id);
}

void VertexArray::addVertexBuffer(const VertexBuffer& buffer)
{
    buffer.bind();
    int i = 0;
    for (m_attribCount ; m_attribCount < buffer.getLayout().size() ; m_attribCount++)
    {
        glVertexAttribPointer(m_attribCount, 
                              buffer.getLayout()[i].componentCount(), 
                              buffer.getLayout()[i].getOpenGLType(), 
                              GL_FALSE, 
                              buffer.getLayout().getStride(), 
                              (const void*)buffer.getLayout()[i].offset);

        glEnableVertexAttribArray(m_attribCount);
        i++;
    }
}

void VertexArray::setIndexBuffer(const IndexBuffer& buffer)
{
    bind();

    buffer.bind();

    m_pIndexBuffer = &buffer;
}