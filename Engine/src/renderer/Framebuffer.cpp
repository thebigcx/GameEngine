#include <renderer/Framebuffer.h>
#include <platform/GL/GLFramebuffer.h>

#include <iostream>

namespace Engine
{

Reference<Renderbuffer> Renderbuffer::create(uint32_t width, uint32_t height, GLenum internalFormat)
{
    return createReference<GLRenderbuffer>(width, height, internalFormat);
}

Reference<Framebuffer> Framebuffer::create(uint32_t width, uint32_t height)
{
    return createReference<GLFramebuffer>(width, height);
}

Reference<Framebuffer> Framebuffer::create()
{
    return createReference<GLFramebuffer>();
}

Reference<Framebuffer> Framebuffer::create(const Specification& spec)
{
    return createReference<GLFramebuffer>(spec);
}

}