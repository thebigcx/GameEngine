#include <renderer/Framebuffer.h>
#include <platform/GL/GLFramebuffer.h>

#include <GL/glew.h>

#include <iostream>

Shared<Framebuffer> Framebuffer::create(uint32_t width, uint32_t height)
{
    return createShared<GLFramebuffer>(width, height);
}