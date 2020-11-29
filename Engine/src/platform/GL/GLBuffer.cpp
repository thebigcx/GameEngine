#include <platform/GL/GLBuffer.h>

#include "renderer/Buffer.h"

#include <GL/glew.h>

GLVertexBuffer::GLVertexBuffer(size_t size)
{
    glCreateBuffers(1, &m_id);

    bind();

    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

    m_usage = BufferUsage::Dynamic;
}

GLVertexBuffer::GLVertexBuffer(const void* data, size_t size)
{
    glCreateBuffers(1, &m_id);

    bind();

    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    m_usage = BufferUsage::Static;
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void GLVertexBuffer::setData(const void* data, size_t size, size_t offset)
{
    bind();

    if (m_usage == BufferUsage::Static)
    {
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }
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

GLIndexBuffer::GLIndexBuffer(uint32_t count)
{
    glCreateBuffers(1, &m_id);

    m_count = count;

    bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);

    m_usage = BufferUsage::Dynamic;
}

GLIndexBuffer::GLIndexBuffer(const uint32_t* data, uint32_t count)
{
    m_count = count;

    glCreateBuffers(1, &m_id);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);

    m_usage = BufferUsage::Static;
}

GLIndexBuffer::~GLIndexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void GLIndexBuffer::setData(const uint32_t* data, uint32_t count, uint32_t offset)
{
    bind();

    if (m_usage == BufferUsage::Static)
    {
        m_count = count;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
    }
    else
    {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(uint32_t), count * sizeof(uint32_t), data);
    }
}

void GLIndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void GLIndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexDataType GLIndexBuffer::getDataType() const
{
    return IndexDataType::UInt32;
}

//------------------------------------------------------------------------------------------------//

GLUniformBuffer::GLUniformBuffer(size_t size, uint32_t bindingPoint)
{
    m_bindingPoint = bindingPoint;

    glCreateBuffers(1, &m_id);

    glBindBuffer(GL_UNIFORM_BUFFER, m_id);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, m_bindingPoint, m_id, 0, size);

    m_usage = BufferUsage::Dynamic;
}

GLUniformBuffer::GLUniformBuffer(const void* data, size_t size, uint32_t bindingPoint)
{
    m_bindingPoint = bindingPoint;

    glCreateBuffers(1, &m_id);

    glBindBuffer(GL_UNIFORM_BUFFER, m_id);
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, m_bindingPoint, m_id, 0, size);

    m_usage = BufferUsage::Static;
}

void GLUniformBuffer::bind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_id);
}

void GLUniformBuffer::unbind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformBuffer::setData(const void* data, size_t size, size_t offset)
{
    bind();

    if (m_usage == BufferUsage::Static)
    {
        glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
    }
    else
    {
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    }

    unbind();
}