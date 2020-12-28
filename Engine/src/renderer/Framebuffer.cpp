#include <renderer/Framebuffer.h>
#include <platform/GL/GLFramebuffer.h>

#include <iostream>

namespace Engine
{

Shared<Framebuffer> Framebuffer::create(uint32_t width, uint32_t height)
{
    return createShared<GLFramebuffer>(width, height);
}

Shared<Framebuffer> Framebuffer::create(const Shared<Texture2D>& texture, GLenum attachment)
{
    return createShared<GLFramebuffer>(static_cast<const GLTexture2D&>(*texture), attachment);
}

}