#pragma once

#include <cstdlib>

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