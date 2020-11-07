#pragma once

#include <cstdlib>
#include <string>
#include <vector>

#include <renderer/shader/Shader.h>
#include <core/Console.h>

struct BufferElement
{
    BufferElement(Shader::DataType type, const std::string& name)
        : type(type), name(name)
    {
        // TODO: add support for other types besides floats
        size = dataTypeSize();
    }

    int componentCount() const
    {
        switch (type)
        {
            case Shader::DataType::Float:   return 1;
            case Shader::DataType::Vec2:    return 2;
            case Shader::DataType::Vec3:    return 3;
            case Shader::DataType::Mat3:    return 9;
            case Shader::DataType::Mat4:    return 16;
            case Shader::DataType::Bool:    return 1;
            case Shader::DataType::Int:     return 1;
            case Shader::DataType::Vec2i:   return 2;
            case Shader::DataType::Vec3i:   return 3;
            case Shader::DataType::Color:   return 4;
            default:                        return 0;
        }
    }

    size_t dataTypeSize()
    {
        size_t size = 0;
        
        if (getOpenGLType() == GL_FLOAT)
            size = sizeof(float);
        else if (getOpenGLType() == GL_INT)
            size = sizeof(int);

        return componentCount() * size;
    }

    GLenum getOpenGLType() const
    {
        switch (type)
        {
            case Shader::DataType::Float:   
            case Shader::DataType::Vec2:
            case Shader::DataType::Vec3:
            case Shader::DataType::Mat3:
            case Shader::DataType::Mat4:
            case Shader::DataType::Color:   return GL_FLOAT;
            case Shader::DataType::Bool:
            case Shader::DataType::Int:
            case Shader::DataType::Vec2i:
            case Shader::DataType::Vec3i:   return GL_INT;
            default:                        return GL_FLOAT;
        }
    }

    Shader::DataType type;
    std::string name;
    size_t offset;
    size_t size;
};



class BufferLayout
{
public:
    BufferLayout()
    {

    }

    BufferLayout(const std::initializer_list<BufferElement>& elements)
        : m_elements(elements)
    {
        setOffsets();
    }

    void setOffsets()
    {
        size_t offset = 0;
        m_stride = 0;
        for (auto& element : m_elements)
        {
            element.offset = offset;
            offset += element.size;
            m_stride += element.size;
        }
    }

    int size() const
    {
        return m_elements.size();
    }

    const BufferElement& operator[](int index) const
    {
        return m_elements[index];
    }

    size_t getStride() const
    {
        return m_stride;
    }

private:
    std::vector<BufferElement> m_elements;

    size_t m_stride;
};



class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

    void update(const void* data, size_t size);

    void bind() const;

    void create(size_t size);

    void setLayout(const BufferLayout& layout);
    const BufferLayout& getLayout() const;

private:
    unsigned int m_id;

    BufferLayout m_layout;
};

class IndexBuffer
{
public:
    IndexBuffer();
    ~IndexBuffer();

    void update(const unsigned int* data, unsigned int size);

    void create(unsigned int size);

    void bind() const;

    inline unsigned int getCount() const
    {
        return m_count;
    }

    inline GLenum getIndexType() const
    {
        return m_indexType;
    }

private:
    unsigned int m_id = 0;

    unsigned int m_count = 0;

    const GLenum m_indexType = GL_UNSIGNED_INT;
};