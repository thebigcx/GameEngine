#include <platform/GL/GLBuffer.h>

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

GLIndexBuffer::GLIndexBuffer(uint32_t count)
{
    glCreateBuffers(1, &m_id);

    m_count = count;

    bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
}

GLIndexBuffer::GLIndexBuffer(const uint32_t* data, uint32_t count)
{
    m_count = count;

    glCreateBuffers(1, &m_id);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
}

GLIndexBuffer::~GLIndexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void GLIndexBuffer::update(const uint32_t* data, uint32_t count)
{
    bind();

    //if (count < m_count)
    {
        m_count = count;
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
    }
    //else
    {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32_t), data);
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
    return IndexDataType::UnsignedInt;
}

//------------------------------------------------------------------------------------------------//
// TODO: add multiple binding points

GLUniformBuffer::GLUniformBuffer(size_t size)
{
    glCreateBuffers(1, &m_id);

    glBindBuffer(GL_UNIFORM_BUFFER, m_id);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_id, 0, size);
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

    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);

    unbind();
}

void GLUniformBuffer::addBinding(const Shared<Shader>& shader, const std::string& name)
{
    bind();

    uint32_t index = glGetUniformBlockIndex(shader->getId(), name.c_str());
    glUniformBlockBinding(shader->getId(), index, 0);

    unbind();
}