#include <platform/GL/GLContext.h>
#include <renderer/RenderingContext.h>

namespace Engine
{

Unique<RenderingContext> RenderingContext::create(GLFWwindow* window)
{
    return createUnique<GLContext>(window);
}

}