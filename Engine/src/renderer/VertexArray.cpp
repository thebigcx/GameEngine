#include <renderer/VertexArray.h>
#include <platform/GL/GLVertexArray.h>

namespace Engine
{

Reference<VertexArray> VertexArray::create()
{
    return createReference<GLVertexArray>();
}

}