#include <renderer/Buffer.h>
#include <platform/GL/GLBuffer.h>

namespace Engine
{

Shared<VertexBuffer> VertexBuffer::create(size_t size)
{
    return createShared<GLVertexBuffer>(size);
}

Shared<IndexBuffer> IndexBuffer::create(uint32_t count, IndexDataType type)
{
    return createShared<GLIndexBuffer>(count, type);
}

Shared<IndexBuffer> IndexBuffer::create(const uint32_t* data, uint32_t count, IndexDataType type)
{
    return createShared<GLIndexBuffer>(data, count, type);
}

Shared<UniformBuffer> UniformBuffer::create(size_t size, uint32_t bindingPoint)
{
    return createShared<GLUniformBuffer>(size, bindingPoint);
}

}