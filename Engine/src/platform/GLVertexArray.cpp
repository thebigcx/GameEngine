#include <platform/GLVertexArray.h>

GLVertexArray::GLVertexArray()
{
    glCreateVertexArrays(1, &m_id);
}

GLVertexArray::~GLVertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

void GLVertexArray::bind() const
{
    glBindVertexArray(m_id);
}

void GLVertexArray::unbind() const
{
    glBindVertexArray(0);
}

void GLVertexArray::addVertexBuffer(Shared<VertexBuffer> buffer)
{
    bind();
    buffer->bind();
    int i = 0;
    for (m_attribCount ; m_attribCount < buffer->getLayout().size() ; m_attribCount++)
    {
        glVertexAttribPointer(m_attribCount, 
                              buffer->getLayout()[i].componentCount(), 
                              buffer->getLayout()[i].getOpenGLType(), 
                              GL_FALSE, 
                              buffer->getLayout().getStride(), 
                              (const void*)buffer->getLayout()[i].offset);

        glEnableVertexAttribArray(m_attribCount);
        i++;
    }
}

void GLVertexArray::setIndexBuffer(Shared<IndexBuffer> buffer)
{
    bind();

    buffer->bind();

    m_pIndexBuffer = buffer;
}

Shared<IndexBuffer> GLVertexArray::getIndexBuffer() const
{
    return m_pIndexBuffer;
}