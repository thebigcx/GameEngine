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
    unsigned int m_id = 0;

    BufferLayout m_layout;
};

class GLIndexBuffer : public IndexBuffer
{
public:
    GLIndexBuffer(unsigned int count);
    ~GLIndexBuffer();

    void update(const unsigned int* data, unsigned int size) override;

    void bind() const override;
    void unbind() const override;

    inline unsigned int getCount() const override
    {
        return m_count;
    }

private:
    unsigned int m_id = 0;
    unsigned int m_count = 0;
};