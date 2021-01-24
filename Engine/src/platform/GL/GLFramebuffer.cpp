#include <platform/GL/GLFramebuffer.h>
#include <core/Logger.h>

#include <GL/glew.h>

#include <iostream>

namespace Engine
{

GLRenderbuffer::GLRenderbuffer(uint32_t width, uint32_t height, GLenum internalFormat)
    : m_width(width), m_height(height)
{
    glCreateRenderbuffers(1, &m_id);
    glBindRenderbuffer(GL_RENDERBUFFER, m_id);
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

GLRenderbuffer::~GLRenderbuffer()
{
    glDeleteRenderbuffers(1, &m_id);
}

void GLRenderbuffer::bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_id);
}

void GLRenderbuffer::unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
// TODO: add multisample
GLFramebuffer::GLFramebuffer()
{
    glCreateFramebuffers(1, &m_id);
}

GLFramebuffer::GLFramebuffer(const Specification& spec)
    : m_width(spec.width), m_height(spec.height)
{
    glCreateFramebuffers(1, &m_id);

    bind();

    for (auto& attachment : spec.attachments)
    {
        if (attachment.attachment == Attachment::Color)
        {
            m_colorAttachmentSpecs.emplace_back(attachment.texture);
        }
        else if (attachment.attachment == Attachment::Depth)
        {
            m_depthAttachmentSpec = attachment.texture;
        }
    }

    invalidate();
}

GLFramebuffer::GLFramebuffer(uint32_t width, uint32_t height)
    : m_width(width), m_height(height)
{
    invalidate();
}

GLFramebuffer::~GLFramebuffer()
{
    if (m_id != 0)
    {
        glDeleteFramebuffers(1, &m_id);
        glDeleteTextures(m_colorAttachments.size(), &m_colorAttachments[0]);
        glDeleteTextures(1, &m_depthAttachment);
    }
}

void GLFramebuffer::attachRenderbuffer(const Renderbuffer& buffer, Attachment attachment)
{
    bind();
    buffer.bind();

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, getAttachmentEnumValue_(attachment), GL_RENDERBUFFER, buffer.getId());
    m_width = buffer.getWidth();
    m_height = buffer.getHeight();

    buffer.unbind();
    unbind();
}

void GLFramebuffer::attachTexture(const Texture2D& texture, Attachment attachment, GLenum target)
{
    bind();

    texture.bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, getAttachmentEnumValue_(attachment), target, texture.getId(), 0);

    unbind();
}

void GLFramebuffer::invalidate()
{
    if (m_id != 0)
    {
        glDeleteFramebuffers(1, &m_id);
        glDeleteTextures(m_colorAttachments.size(), &m_colorAttachments[0]);
        glDeleteTextures(1, &m_depthAttachment);

        m_colorAttachments.clear();
        m_depthAttachment = 0;
    }
    
    // Create and bind the framebuffer
    glCreateFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    if (m_colorAttachmentSpecs.size() > 0)
    {
        m_colorAttachments.resize(m_colorAttachmentSpecs.size());
        glCreateTextures(GL_TEXTURE_2D, m_colorAttachments.size(), &m_colorAttachments[0]);

        for (size_t i = 0; i < m_colorAttachments.size(); i++)
        {
            auto format = getTextureFormatEnumValue_(m_colorAttachmentSpecs[i].format);

            glBindTextureUnit(0, m_colorAttachments[i]);
            glTextureStorage2D(m_colorAttachments[i], 1, format, m_width, m_height);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorAttachments[i], 0);
        }
    }

    if (m_depthAttachmentSpec.format != SizedTextureFormat::None)
    {
        auto format = getTextureFormatEnumValue_(m_depthAttachmentSpec.format);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment);
        glBindTextureUnit(0, m_depthAttachment);
        glTextureStorage2D(m_depthAttachment, 1, format, m_width, m_height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);
    }
    
    if (m_colorAttachments.size() > 1)
    {
        assert(m_colorAttachments.size() <= 4);
        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(m_colorAttachments.size(), buffers);
    }
    else if (m_colorAttachments.empty())
    {
        glDrawBuffer(GL_NONE);
    }
    

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

    invalidate();
}

void GLFramebuffer::bind() const
{
    s_currentBound = this;
    glViewport(0, 0, m_width, m_height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void GLFramebuffer::unbind() const
{
    s_currentBound = nullptr;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFramebuffer::drawBuffer(uint32_t buffer) const
{
    bind();
    glDrawBuffer(getColorBufferEnumValue_(buffer));
    unbind();
}

void GLFramebuffer::readBuffer(uint32_t buffer) const
{
    bind();
    glReadBuffer(getColorBufferEnumValue_(buffer));
    unbind();
}

GLenum GLFramebuffer::getColorBufferEnumValue_(uint32_t buffer)
{
    bool bl = false, br = false, fl = false, fr = false;

    if (buffer & static_cast<uint32_t>(ColorBuffer::FrontLeft))
        fl = true;

    if (buffer & static_cast<uint32_t>(ColorBuffer::FrontRight))
        fr = true;

    if (buffer & static_cast<uint32_t>(ColorBuffer::BackLeft))
        bl = true;

    if (buffer & static_cast<uint32_t>(ColorBuffer::BackRight))
        br = true;

    if (!bl && !br && !fl && !fr)
        return GL_NONE;
    else if (!bl && !br && fl && !fr)
        return GL_FRONT_LEFT;
    else if (!bl && !br && !fl && fr)
        return GL_FRONT_RIGHT;
    else if (bl && !br && !fl && !fr)
        return GL_BACK_LEFT;
    else if (!bl && br && !fl && !fr)
        return GL_BACK_RIGHT;
    else if (!bl && !br && fl && fr)
        return GL_FRONT;
    else if (bl && br && !fl && !fr)
        return GL_BACK;
    else if (bl && !br && fl && !fr)
        return GL_LEFT;
    else if (!bl && br && !fl && fr)
        return GL_RIGHT;
    else if (bl && br && fl && fr)
        return GL_FRONT_AND_BACK;

    return 0;
}

GLenum GLFramebuffer::getAttachmentEnumValue_(Attachment attachment)
{
    switch (attachment)
    {
        case Attachment::Color: return GL_COLOR_ATTACHMENT0;
        case Attachment::Depth: return GL_DEPTH_ATTACHMENT;
        case Attachment::Stencil: return GL_STENCIL_ATTACHMENT;
        case Attachment::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
        default: return GL_NONE;
    };
}

GLenum GLFramebuffer::getTextureFormatEnumValue_(SizedTextureFormat format)
{
    switch (format)
    {
        case SizedTextureFormat::RGBA8: return GL_RGBA8;
        case SizedTextureFormat::Depth16: return GL_DEPTH_COMPONENT16;
        case SizedTextureFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
        default: return GL_NONE;
    };
}

}