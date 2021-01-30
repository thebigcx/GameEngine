#include <renderer/RendererAPI.h>
#include <platform/GL/GLRendererAPI.h>

namespace Engine
{

Owned<RendererAPI> RendererAPI::create()
{
    return createOwned<GLRendererAPI>();
}

}