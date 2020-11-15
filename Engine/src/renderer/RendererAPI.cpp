#include <renderer/RendererAPI.h>
#include <platform/GL/GLRendererAPI.h>

Unique<RendererAPI> RendererAPI::create()
{
    return createUnique<GLRendererAPI>();
}