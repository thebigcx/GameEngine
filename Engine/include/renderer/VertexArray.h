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

    virtual void addVertexBuffer(const Shared<VertexBuffer>& buffer) = 0;
    virtual void setIndexBuffer(const Shared<IndexBuffer>& buffer) = 0;

    virtual Shared<IndexBuffer> getIndexBuffer() const = 0;

    static Shared<VertexArray> create();

    virtual bool operator==(const VertexArray& array) const = 0;
    virtual bool operator!=(const VertexArray& array) const = 0;
};

}