#include <platform/GL/GLContext.h>
#include <renderer/RenderingContext.h>

Unique<RenderingContext> RenderingContext::create(GLFWwindow* window)
{
    return createUnique<GLContext>(window);
}