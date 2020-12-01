#pragma once

#include <cstdlib>

#include <GL/glew.h>

#include <renderer/Buffer.h>

class GLVertexBuffer : public VertexBuffer
{
public:
    GLVertexBuffer(size_t size); // Dynamic
    GLVertexBuffer(const void* data, size_t size); // Static
    ~GLVertexBuffer();

    void setData(const void* data, size_t size, size_t offset = 0) override;

    void bind() const override;
    void unbind() const override;

    void setLayout(const BufferLayout& layout) override;

    void* getBufferPtr(size_t size, size_t offset) const override;
    void* getBufferPtr(size_t offset) const override;
    void unmap() const override;
    
    inline const BufferLayout& getLayout() const override
    {
        return m_layout;
    }

private:
    uint32_t m_id = 0;

    size_t m_size;

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

    IndexDataType getDataType() const override;

    void* getBufferPtr(uint32_t size, uint32_t offset) const override;
    void* getBufferPtr(uint32_t offset) const override;
    void unmap() const override;

    inline uint32_t getCount() const override
    {
        return m_count;
    }

private:
    uint32_t m_id = 0;
    uint32_t m_count = 0;

    size_t m_typeSize;

    BufferUsage m_usage;

    static size_t calculateTypeSize(IndexDataType type);
};

class GLUniformBuffer : public UniformBuffer
{
public:
    GLUniformBuffer(size_t size, uint32_t bindingPoint); // Dynamic
    GLUniformBuffer(const void* data, size_t size, uint32_t bindingPoint); // Static

    void setData(const void* data, size_t size, size_t offset = 0) override;

    void bind() const override;
    void unbind() const override;

    void* getBufferPtr(size_t size, size_t offset) const override;
    void* getBufferPtr(size_t offset) const override;
    void unmap() const override;

private:
    uint32_t m_id;
    uint32_t m_bindingPoint;

    BufferUsage m_usage;

    size_t m_size;
};