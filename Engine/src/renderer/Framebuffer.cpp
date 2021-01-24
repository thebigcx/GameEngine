#include <renderer/Framebuffer.h>
#include <platform/GL/GLFramebuffer.h>

#include <iostream>

namespace Engine
{

Shared<Renderbuffer> Renderbuffer::create(uint32_t width, uint32_t height, GLenum internalFormat)
{
    return createShared<GLRenderbuffer>(width, height, internalFormat);
}

Shared<Framebuffer> Framebuffer::create(uint32_t width, uint32_t height)
{
    return createShared<GLFramebuffer>(width, height);
}

Shared<Framebuffer> Framebuffer::create()
{
    return createShared<GLFramebuffer>();
}

Shared<Framebuffer> Framebuffer::create(const FramebufferSpec& spec)
{
    return createShared<GLFramebuffer>(spec);
}

}