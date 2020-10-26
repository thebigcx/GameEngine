#include "renderer/Buffer.h"

#include <GL/glew.h>

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_id);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void VertexBuffer::update(const void* data, size_t size)
{
    bind();

    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void VertexBuffer::create(size_t size)
{
    bind();

    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

void VertexBuffer::setLayout(const BufferLayout& layout)
{
    m_layout = layout;
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

const BufferLayout& VertexBuffer::getLayout() const
{
    return m_layout;
}

//------------------------------------------------------------------------------------------------//

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &m_id);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void IndexBuffer::update(const unsigned int* data, unsigned int count)
{
    m_count = count;

    bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

void IndexBuffer::create(unsigned int count)
{
    m_count = count;

    bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}