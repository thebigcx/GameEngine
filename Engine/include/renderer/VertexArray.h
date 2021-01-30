#pragma once

#include <GL/glew.h>

#include <renderer/Buffer.h>

namespace Engine
{

class VertexArray
{
public:
    virtual ~VertexArray() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void addVertexBuffer(const Reference<VertexBuffer>& buffer) = 0;
    virtual void setIndexBuffer(const Reference<IndexBuffer>& buffer) = 0;

    virtual Reference<IndexBuffer> getIndexBuffer() const = 0;

    static Reference<VertexArray> create();

    virtual bool operator==(const VertexArray& array) const = 0;
    virtual bool operator!=(const VertexArray& array) const = 0;
};

}