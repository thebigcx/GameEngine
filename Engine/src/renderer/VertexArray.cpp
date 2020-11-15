#include <renderer/VertexArray.h>
#include <platform/GL/GLVertexArray.h>

Shared<VertexArray> VertexArray::create()
{
    return createShared<GLVertexArray>();
}