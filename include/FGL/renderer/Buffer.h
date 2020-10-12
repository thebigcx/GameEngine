#pragma once

#include <cstdlib>

class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

    void update(const void* data, size_t size);

    void bind() const;

    void allocate(size_t size);

private:
    unsigned int m_id;
};

class IndexBuffer
{
public:
    IndexBuffer();
    ~IndexBuffer();

    void update(const unsigned int* data, unsigned int size);

    void bind() const;

    inline unsigned int getCount() const
    {
        return m_count;
    }

private:
    unsigned int m_id;

    unsigned int m_count;
};