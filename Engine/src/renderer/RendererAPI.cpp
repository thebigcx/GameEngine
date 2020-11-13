#include <renderer/RendererAPI.h>
#include <platform/GLRendererAPI.h>

Unique<RendererAPI> RendererAPI::create()
{
    return createUnique<GLRendererAPI>();
}