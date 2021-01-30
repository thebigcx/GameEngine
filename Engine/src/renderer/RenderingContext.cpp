#include <platform/GL/GLContext.h>
#include <renderer/RenderingContext.h>

namespace Engine
{

Owned<RenderingContext> RenderingContext::create(void* window)
{
    return createOwned<GLContext>(static_cast<GLFWwindow*>(window));
}

}