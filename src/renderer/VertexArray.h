#pragma once

#include <GL/glew.h>

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void bind() const;

private:
    unsigned int m_id;
};