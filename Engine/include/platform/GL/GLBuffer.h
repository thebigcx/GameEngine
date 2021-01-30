#pragma once

#include <cstdlib>

#include <renderer/Buffer.h>

namespace Engine
{

class GLVertexBuffer : public VertexBuffer
{
public:
    GLVertexBuffer(size_t size); // Dynamic
    GLVertexBuffer(const void* data, size_t size); // Static
    ~GLVertexBuffer();

    void setData(const void* data, size_t size, size_t offset = 0) override;

    void bind() const override;
    void unbind() const override;

    void* getBufferPtr(size_t size, size_t offset) const override;
    void* getBufferPtr(size_t offset) const override;
    void unmap() const override;
    
    inline void setLayout(const BufferLayout& layout) override
    {
        m_layout = layout;
    }

    inline const BufferLayout& getLayout() const override
    {
        return m_layout;
    }

private:
    uint32_t m_id = 0;

    size_t m_size = 0;

    BufferLayout m_layout;

    BufferUsage m_usage;
};

class GLIndexBuffer : public IndexBuffer
{
public:
    GLIndexBuffer(uint32_t count, IndexDataType type); // Dynamic
    GLIndexBuffer(const uint32_t* data, uint32_t count, IndexDataType type); // Static
    ~GLIndexBuffer();

    void setData(const uint32_t* data, uint32_t count, uint32_t offset = 0) override;

    void bind() const override;
    void unbind() const override;

    void* getBufferPtr(uint32_t size, uint32_t offset) const override;
    void* getBufferPtr(uint32_t offset) const override;
    void unmap() const override;

    inline IndexDataType getDataType() const noexcept override
    {
        return IndexDataType::UInt32;
    }

    inline uint32_t getCount() const noexcept override
    {
        return m_count;
    }

private:
    uint32_t m_id = 0;
    uint32_t m_count = 0;

    size_t m_typeSize = 0;

    BufferUsage m_usage;

    static size_t calculateTypeSize(IndexDataType type);
};

class GLUniformBuffer : public UniformBuffer
{
public:
    GLUniformBuffer(size_t size, uint32_t bindingPoint); // Dynamic
    GLUniformBuffer(const void* data, size_t size, uint32_t bindingPoint); // Static

    void setBlockDeclaration(const Shader& shader) override;

    void setData(const void* data, size_t size, size_t offset = 0) override;

    void setVariable(const std::string& uniform, const void* data, size_t size) override;

    void bind() const override;
    void unbind() const override;

    void* getBufferPtr(size_t size, size_t offset) const override;
    void* getBufferPtr(size_t offset) const override;
    void unmap() const override;

    size_t getVariableOffset(const std::string& name) const override;

private:
    uint32_t m_id = 0;
    uint32_t m_bindingPoint = 0;

    BufferUsage m_usage;

    size_t m_size = 0;

    std::unordered_map<std::string, size_t> m_variableOffsets;
};

}