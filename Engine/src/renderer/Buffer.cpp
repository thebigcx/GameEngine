#include <renderer/Buffer.h>
#include <platform/GLBuffer.h>

Shared<VertexBuffer> VertexBuffer::create(size_t size)
{
    return createShared<GLVertexBuffer>(size);
}

Shared<IndexBuffer> IndexBuffer::create(unsigned int count)
{
    return createShared<GLIndexBuffer>(count);
}