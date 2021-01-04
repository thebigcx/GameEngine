#pragma once

#include <cstdlib>
#include <string>
#include <vector>

#include <renderer/shader/Shader.h>

namespace Engine
{

struct BufferElement
{
    BufferElement(Shader::DataType type, const std::string& name, bool normalized = false)
        : type(type), name(name), normalized(normalized)
    {
        size = dataTypeSize();
    }

    inline uint32_t componentCount() const
    {
        switch (type)
        {
            using Type = Shader::DataType;

            case Type::Float:
            case Type::Bool:
            case Type::Int:
            case Type::Double:
            case Type::UInt:  return 1;

            case Type::Float2:
            case Type::Int2:
            case Type::UInt2:
            case Type::Double2:
            case Type::Bool2: return 2;

            case Type::Float3:
            case Type::Int3:
            case Type::UInt3:
            case Type::Double3:
            case Type::Bool3: return 3;

            case Type::Float4:
            case Type::Int4:
            case Type::UInt4:
            case Type::Double4:
            case Type::Bool4: return 4;

            case Type::Mat2:
            case Type::Mat2x2:
            case Type::DMat2:
            case Type::DMat2x2: return 2;

            case Type::Mat2x3:
            case Type::DMat2x3: return 2;

            case Type::Mat2x4:
            case Type::DMat2x4: return 2;

            case Type::Mat3x2:
            case Type::DMat3x2: return 3;

            case Type::Mat3:
            case Type::Mat3x3:
            case Type::DMat3: 
            case Type::DMat3x3: return 3;

            case Type::Mat3x4:
            case Type::DMat3x4: return 3;

            case Type::Mat4x2:
            case Type::DMat4x2: return 4;

            case Type::Mat4x3:
            case Type::DMat4x3: return 4;

            case Type::Mat4:
            case Type::Mat4x4:
            case Type::DMat4:
            case Type::DMat4x4: return 4;

            default:          return 0;
        }
    }

    inline size_t dataTypeSize()
    {
        switch (type)
        {
            using Type = Shader::DataType;

            case Type::Float:
            case Type::Float2:
            case Type::Float3:
            case Type::Float4: return componentCount() * sizeof(float);
            case Type::Mat2:   return 2 * 2 * sizeof(float);
            case Type::Mat2x2: return 2 * 2 * sizeof(float);
            case Type::Mat2x3: return 2 * 3 * sizeof(float);
            case Type::Mat2x4: return 2 * 4 * sizeof(float);
            case Type::Mat3x2: return 3 * 2 * sizeof(float);
            case Type::Mat3:   return 3 * 3 * sizeof(float);
            case Type::Mat3x3: return 3 * 3 * sizeof(float);
            case Type::Mat3x4: return 3 * 4 * sizeof(float);
            case Type::Mat4x2: return 4 * 2 * sizeof(float);
            case Type::Mat4x3: return 4 * 3 * sizeof(float);
            case Type::Mat4:   return 4 * 4 * sizeof(float);
            case Type::Mat4x4: return 4 * 4 * sizeof(float);

            case Type::Bool:
            case Type::Bool2:
            case Type::Bool3:
            case Type::Bool4:
            case Type::Int:
            case Type::Int2:
            case Type::Int3:
            case Type::Int4: return componentCount() * sizeof(int);

            case Type::UInt:
            case Type::UInt2:
            case Type::UInt3:
            case Type::UInt4: return componentCount() * sizeof(uint32_t);

            case Type::Double:
            case Type::Double2:
            case Type::Double3:
            case Type::Double4:
            case Type::DMat2:   return 2 * 2 * sizeof(double);
            case Type::DMat2x2: return 2 * 2 * sizeof(double);
            case Type::DMat2x3: return 2 * 3 * sizeof(double);
            case Type::DMat2x4: return 2 * 4 * sizeof(double);
            case Type::DMat3x2: return 3 * 2 * sizeof(double);
            case Type::DMat3:   return 3 * 3 * sizeof(double);
            case Type::DMat3x3: return 3 * 3 * sizeof(double);
            case Type::DMat3x4: return 3 * 4 * sizeof(double);
            case Type::DMat4x2: return 4 * 2 * sizeof(double);
            case Type::DMat4x3: return 4 * 3 * sizeof(double);
            case Type::DMat4:   return 4 * 4 * sizeof(double);
            case Type::DMat4x4: return 4 * 4 * sizeof(double);

            default:          return 0;
        }
    }

    Shader::DataType type;
    std::string name;
    size_t offset;
    size_t size;
    bool normalized;
};


class BufferLayout
{
public:
    BufferLayout() = default;

    BufferLayout(const std::initializer_list<BufferElement>& elements)
        : m_elements(elements)
    {
        setOffsets();
    }

    inline void setOffsets()
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

    inline uint32_t size() const
    {
        return m_elements.size();
    }

    inline const BufferElement& operator[](uint32_t index) const
    {
        return m_elements[index];
    }

    inline constexpr size_t getStride() const
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

    static Shared<VertexBuffer> create(size_t size = 0);
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

}