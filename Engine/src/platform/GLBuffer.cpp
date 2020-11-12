#include <platform/GLBuffer.h>

#include "renderer/Buffer.h"

#include <GL/glew.h>

GLVertexBuffer::GLVertexBuffer(size_t size)
{
    glCreateBuffers(1, &m_id);

    bind();

    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void GLVertexBuffer::update(const void* data, size_t size)
{
    bind();

    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void GLVertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void GLVertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLVertexBuffer::setLayout(const BufferLayout& layout)
{
    m_layout = layout;
}

//------------------------------------------------------------------------------------------------//

GLIndexBuffer::GLIndexBuffer(unsigned int count)
{
    glCreateBuffers(1, &m_id);

    m_count = count;

    bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);
}

GLIndexBuffer::~GLIndexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void GLIndexBuffer::update(const unsigned int* data, unsigned int count)
{
    m_count = count;

    bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

void GLIndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void GLIndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}