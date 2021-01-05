#include <platform/GL/GLBuffer.h>
#include <renderer/Buffer.h>
#include <core/Logger.h>

#include <cstring>

#include <GL/glew.h>

namespace Engine
{

GLVertexBuffer::GLVertexBuffer(size_t size)
    : m_size(size), m_usage(BufferUsage::Dynamic)
{
    glCreateBuffers(1, &m_id);

    if (size > 0)
    {
        bind();

        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }
    else
    {
        m_usage = BufferUsage::Static;
    }
    
}

GLVertexBuffer::GLVertexBuffer(const void* data, size_t size)
    : m_size(size), m_usage(BufferUsage::Static)
{
    glCreateBuffers(1, &m_id);

    bind();

    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
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
        m_size = size;
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
    else
    {
        void* ptr = getBufferPtr(size, offset);
        memcpy(ptr, data, size);
        unmap();
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

void* GLVertexBuffer::getBufferPtr(size_t size, size_t offset) const
{
    void* ptr = glMapNamedBufferRange(m_id, offset, size, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

    if (ptr == nullptr)
    {
        Logger::getCoreLogger()->error("Nullptr returned from glMapNamedBufferRange().");
    }

    return ptr;
}

void* GLVertexBuffer::getBufferPtr(size_t offset) const
{
    return getBufferPtr(m_size - offset, offset);
}

void GLVertexBuffer::unmap() const
{
    glUnmapNamedBuffer(m_id);
}

//------------------------------------------------------------------------------------------------//

GLIndexBuffer::GLIndexBuffer(uint32_t count, IndexDataType type)
    : m_count(count), m_usage(BufferUsage::Dynamic)
{
    m_typeSize = calculateTypeSize(type);

    glCreateBuffers(1, &m_id);

    bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * m_typeSize, nullptr, GL_DYNAMIC_DRAW);
}

GLIndexBuffer::GLIndexBuffer(const uint32_t* data, uint32_t count, IndexDataType type)
    : m_count(count), m_usage(BufferUsage::Static)
{
    m_typeSize = calculateTypeSize(type);

    glCreateBuffers(1, &m_id);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * m_typeSize, data, GL_STATIC_DRAW);
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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * m_typeSize, data, GL_STATIC_DRAW);
    }
    else
    {
        void* ptr = getBufferPtr(count, offset);
        memcpy(ptr, data, count * m_typeSize);
        unmap();
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

void* GLIndexBuffer::getBufferPtr(uint32_t size, uint32_t offset) const
{
    void* ptr = glMapNamedBufferRange(m_id, offset * m_typeSize, size * m_typeSize, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

    if (ptr == nullptr)
    {
       Logger::getCoreLogger()->error("Nullptr returned from glMapNamedBufferRange().");
    }

    return ptr;
}

void* GLIndexBuffer::getBufferPtr(uint32_t offset) const
{
    return getBufferPtr(m_count - offset, offset);
}

void GLIndexBuffer::unmap() const
{
    glUnmapNamedBuffer(m_id);
}

size_t GLIndexBuffer::calculateTypeSize(IndexDataType type)
{
    switch (type)
    {
        case IndexDataType::UInt8:
            return sizeof(uint8_t);
            break;
        case IndexDataType::UInt16:
            return sizeof(uint16_t);
            break;
        case IndexDataType::UInt32:
            return sizeof(uint32_t);
            break;
        default:
            Logger::getCoreLogger()->error("Invalid index type!");
            return 0;
    };
}

//------------------------------------------------------------------------------------------------//

GLUniformBuffer::GLUniformBuffer(size_t size, uint32_t bindingPoint)
    : m_size(size), m_bindingPoint(bindingPoint), m_usage(BufferUsage::Dynamic)
{
    glCreateBuffers(1, &m_id);

    glBindBuffer(GL_UNIFORM_BUFFER, m_id);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, m_bindingPoint, m_id, 0, size);
}

GLUniformBuffer::GLUniformBuffer(const void* data, size_t size, uint32_t bindingPoint)
    : m_size(size), m_bindingPoint(bindingPoint), m_usage(BufferUsage::Static)
{
    glCreateBuffers(1, &m_id);

    glBindBuffer(GL_UNIFORM_BUFFER, m_id);
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, m_bindingPoint, m_id, 0, size);
}

void GLUniformBuffer::setBlockDeclaration(const Shader& shader)
{
    shader.bind();
    int32_t count = 0;
    glGetProgramiv(shader.getId(), GL_ACTIVE_UNIFORMS, &count);

    for (int32_t i = 0; i < count; i++)
    {
        GLenum type;
        int32_t size = 0;
        char name[32];
        glGetActiveUniform(shader.getId(), i, 32, nullptr, &size, &type, name);

        GLuint index = i;
        int32_t offset = 0;
        glGetActiveUniformsiv(shader.getId(), 1, &index, GL_UNIFORM_OFFSET, &offset);

        if (offset != -1)
            m_variableOffsets.emplace(std::make_pair(std::string(name), offset));
    }
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
    if (m_usage == BufferUsage::Static)
    {
        bind();
        glBufferData(m_id, size, data, GL_STATIC_DRAW);
        unbind();
    }
    else // Use memcpy (faster)
    {
        void* ptr = getBufferPtr(offset);
        memcpy(ptr, data, size);
        unmap();
    }
}

void* GLUniformBuffer::getBufferPtr(size_t size, size_t offset) const
{
    void* ptr = glMapNamedBufferRange(m_id, offset, size, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

    if (ptr == nullptr)
    {
        Logger::getCoreLogger()->error("Nullptr returned from glMapNamedBufferRange().");
    }

    return ptr;
}

void* GLUniformBuffer::getBufferPtr(size_t offset) const
{
    return getBufferPtr(m_size - offset, offset);
}

void GLUniformBuffer::unmap() const
{
    glUnmapNamedBuffer(m_id);
}

size_t GLUniformBuffer::getVariableOffset(const std::string& name) const
{
    if (m_variableOffsets.find(name) == m_variableOffsets.end())
    {
        return 0;
    }

    return m_variableOffsets.at(name);
}

void GLUniformBuffer::setVariable(const std::string& uniform, const void* data, size_t size)
{
    if (m_variableOffsets.find(uniform) == m_variableOffsets.end())
    {
        return;
    }

    size_t offset = m_variableOffsets.at(uniform);

    setData(data, size, offset);
}

}