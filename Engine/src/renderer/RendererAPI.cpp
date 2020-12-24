#include <renderer/RendererAPI.h>
#include <platform/GL/GLRendererAPI.h>

namespace Engine
{

Unique<RendererAPI> RendererAPI::create()
{
    return createUnique<GLRendererAPI>();
}

}