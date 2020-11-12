#pragma once

#include <GL/glew.h>

#include <renderer/Buffer.h>

class VertexArray
{
public:
    virtual ~VertexArray() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void addVertexBuffer(Shared<VertexBuffer> buffer) = 0;
    virtual void setIndexBuffer(Shared<IndexBuffer> buffer) = 0;

    virtual Shared<IndexBuffer> getIndexBuffer() const = 0;

    static Shared<VertexArray> create();
};