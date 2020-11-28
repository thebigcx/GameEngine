#include <renderer/Buffer.h>
#include <platform/GL/GLBuffer.h>

Shared<VertexBuffer> VertexBuffer::create(size_t size)
{
    return createShared<GLVertexBuffer>(size);
}

Shared<IndexBuffer> IndexBuffer::create(uint32_t count)
{
    return createShared<GLIndexBuffer>(count);
}

Shared<IndexBuffer> IndexBuffer::create(const uint32_t* data, uint32_t count)
{
    return createShared<GLIndexBuffer>(data, count);
}

Shared<UniformBuffer> UniformBuffer::create(size_t size)
{
    return createShared<GLUniformBuffer>(size);
}