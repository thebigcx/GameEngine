#include <platform/GLContext.h>
#include <renderer/GraphicsContext.h>

Unique<GraphicsContext> GraphicsContext::create(GLFWwindow* window)
{
    return createUnique<GLContext>(window);
}