#pragma once

#include <cstdlib>

#include <GL/glew.h>

#include <renderer/Buffer.h>

class GLVertexBuffer : public VertexBuffer
{
public:
    GLVertexBuffer(size_t size);
    ~GLVertexBuffer();

    void update(const void* data, size_t size) override;

    void bind() const override;
    void unbind() const override;

    void setLayout(const BufferLayout& layout) override;
    
    inline const BufferLayout& getLayout() const override
    {
        return m_layout;
    }

private:
    uint32_t m_id = 0;

    BufferLayout m_layout;
};

class GLIndexBuffer : public IndexBuffer
{
public:
    GLIndexBuffer(uint32_t count);
    GLIndexBuffer(const uint32_t* data, uint32_t count);
    ~GLIndexBuffer();

    void update(const uint32_t* data, uint32_t size) override;

    void bind() const override;
    void unbind() const override;

    IndexDataType getDataType() const override;

    inline uint32_t getCount() const override
    {
        return m_count;
    }

private:
    uint32_t m_id = 0;
    uint32_t m_count = 0;
};