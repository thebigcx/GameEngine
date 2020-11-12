#include <renderer/VertexArray.h>
#include <platform/GLVertexArray.h>

Shared<VertexArray> VertexArray::create()
{
    return createShared<GLVertexArray>();
}