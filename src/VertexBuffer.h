#pragma once

#include <cstdlib>

class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

    void update(const void* data, size_t size);

    void bind() const;

private:
    unsigned int m_id;
};