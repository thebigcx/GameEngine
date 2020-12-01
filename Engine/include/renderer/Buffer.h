#pragma once

#include <cstdlib>
#include <string>
#include <vector>

#include <renderer/shader/Shader.h>

struct BufferElement
{
    BufferElement(Shader::DataType type, const std::string& name)
        : type(type), name(name)
    {
        size = dataTypeSize();
    }

    int componentCount() const
    {
        switch (type)
        {
            using Type = Shader::DataType;

            case Type::Float:
            case Type::Bool:
            case Type::Int:
            case Type::Double:
            case Type::Uint:    return 1;

            case Type::Vec2:
            case Type::iVec2:
            case Type::uVec2:
            case Type::dVec2:
            case Type::bVec2:   return 2;

            case Type::Vec3:
            case Type::iVec3:
            case Type::uVec3:
            case Type::dVec3:
            case Type::bVec3:    return 3;

            case Type::Vec4:
            case Type::iVec4:
            case Type::uVec4:
            case Type::dVec4:
            case Type::bVec4:    return 4;

            case Type::Mat3:
            case Type::dMat3:    return 9;

            case Type::Mat4:
            case Type::dMat4:    return 16;

            default:                        return 0;
        }
    }

    size_t dataTypeSize()
    {
        switch (type)
        {
            using Type = Shader::DataType;

            case Type::Float:
            case Type::Vec2:
            case Type::Vec3:
            case Type::Vec4:
            case Type::Mat3:
            case Type::Mat4:    return componentCount() * sizeof(float);

            case Type::Bool:
            case Type::bVec2:
            case Type::bVec3:
            case Type::bVec4:
            case Type::Int:
            case Type::iVec2:
            case Type::iVec3:
            case Type::iVec4:   return componentCount() * sizeof(int);

            case Type::Uint:
            case Type::uVec2:
            case Type::uVec3:
            case Type::uVec4:   return componentCount() * sizeof(unsigned int);

            case Type::Double:
            case Type::dVec2:
            case Type::dVec3:
            case Type::dVec4:
            case Type::dMat3:
            case Type::dMat4:   return componentCount() * sizeof(double);

            default:            return componentCount() * sizeof(float);
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

enum class BufferUsage
{
    Static, Dynamic, Stream
};

class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    virtual void setData(const void* data, size_t size, size_t offset = 0) = 0;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void setLayout(const BufferLayout& layout) = 0;

    virtual const BufferLayout& getLayout() const = 0;

    virtual void* getBufferPtr(size_t size, size_t offset) const = 0;
    virtual void* getBufferPtr(size_t offset) const = 0;
    virtual void unmap() const = 0;

    static Shared<VertexBuffer> create(size_t size);
};

enum class IndexDataType
{
    UInt8,
    UInt16,
    UInt32
};

class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void setData(const uint32_t* data, uint32_t count, uint32_t offset = 0) = 0;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual uint32_t getCount() const = 0;

    virtual IndexDataType getDataType() const = 0;

    virtual void* getBufferPtr(uint32_t size, uint32_t offset) const = 0;
    virtual void* getBufferPtr(uint32_t offset) const = 0;
    virtual void unmap() const = 0;

    static Shared<IndexBuffer> create(uint32_t count, IndexDataType type);
    static Shared<IndexBuffer> create(const uint32_t* data, uint32_t count, IndexDataType type);
};

class UniformBuffer
{
public:
    virtual ~UniformBuffer() = default;

    virtual void setData(const void* data, size_t size, size_t offset = 0) = 0;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void* getBufferPtr(size_t size, size_t offset) const = 0;
    virtual void* getBufferPtr(size_t offset) const = 0;
    virtual void unmap() const = 0;

    static Shared<UniformBuffer> create(size_t size, uint32_t bindingPoint);
};