#include <platform/GL/GLFramebuffer.h>
#include <core/Logger.h>

#include <GL/glew.h>

#include <iostream>

namespace Engine
{

GLFramebuffer::GLFramebuffer(uint32_t width, uint32_t height)
    : m_width(width), m_height(height)
{
    invalidate(width, height);
}

GLFramebuffer::~GLFramebuffer()
{
    if (m_id != 0)
    {
        glDeleteFramebuffers(1, &m_id);
        glDeleteTextures(1, &m_colorAttachment);
        glDeleteTextures(1, &m_depthAttachment);
    }
}

void GLFramebuffer::invalidate(uint32_t width, uint32_t height)
{
    if (m_id != 0)
    {
        glDeleteFramebuffers(1, &m_id);
        glDeleteTextures(1, &m_colorAttachment);
        glDeleteTextures(1, &m_depthAttachment);
    }
     
    // Create and bind the framebuffer
    glCreateFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    // Generate and allocate storage for the color attachment of framebuffer
    glCreateTextures(GL_TEXTURE_2D, 1, &(m_colorAttachment));

    glBindTextureUnit(0, m_colorAttachment);
    glTextureStorage2D(m_colorAttachment, 1, GL_RGBA16F, width, height);

    glTextureParameteri(m_colorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_colorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach the color buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

    // Create and allocate storage for depth attachment
    glCreateTextures(GL_TEXTURE_2D, 1, &(m_depthAttachment));

    glBindTextureUnit(0, m_depthAttachment);
    glTextureStorage2D(m_depthAttachment, 1, GL_DEPTH24_STENCIL8, width, height);

    // Attach the depth buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Logger::getCoreLogger()->error("Framebuffer is incomplete.");
    }

    // Make sure to not leave framebuffer bound
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFramebuffer::resize(uint32_t width, uint32_t height)
{
    if (width > s_maxSize || height > s_maxSize)
    {
        Logger::getCoreLogger()->error("Attempted to resize framebuffer to (%d, %d), which is greater than max size (%d, %d).", width, height, s_maxSize, s_maxSize);
        return;
    }

    m_width = width;
    m_height = height;

    invalidate(width, height);
}

void GLFramebuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    glViewport(0, 0, m_width, m_height);
}

void GLFramebuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}