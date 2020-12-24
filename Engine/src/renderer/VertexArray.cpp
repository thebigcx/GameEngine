#include <renderer/VertexArray.h>
#include <platform/GL/GLVertexArray.h>

namespace Engine
{

Shared<VertexArray> VertexArray::create()
{
    return createShared<GLVertexArray>();
}

}