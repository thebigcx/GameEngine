#include <renderer/Buffer.h>
#include <platform/GL/GLBuffer.h>

namespace Engine
{

Reference<VertexBuffer> VertexBuffer::create(size_t size)
{
    return createReference<GLVertexBuffer>(size);
}

Reference<IndexBuffer> IndexBuffer::create(uint32_t count, IndexDataType type)
{
    return createReference<GLIndexBuffer>(count, type);
}

Reference<IndexBuffer> IndexBuffer::create(const uint32_t* data, uint32_t count, IndexDataType type)
{
    return createReference<GLIndexBuffer>(data, count, type);
}

Reference<UniformBuffer> UniformBuffer::create(size_t size, uint32_t bindingPoint)
{
    return createReference<GLUniformBuffer>(size, bindingPoint);
}

}