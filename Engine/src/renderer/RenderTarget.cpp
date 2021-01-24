#include <renderer/RenderTarget.h>
#include <renderer/RenderCommand.h>

namespace Engine
{

RenderTarget::RenderTarget(uint32_t width, uint32_t height)
    : m_width(width), m_height(height)
{
    invalidate();
}

Shared<RenderTarget> RenderTarget::create(uint32_t width, uint32_t height)
{
    RenderTarget* target = new RenderTarget(width, height);
    return std::shared_ptr<RenderTarget>(target);
}

void RenderTarget::invalidate()
{
    if (m_framebuffer != nullptr)
    {
        // TODO: delete textures and framebuffer object
    }

    FramebufferSpec spec = {
        m_width, m_height,
        {
            { Attachment::Color, FramebufferTextureSpec(FramebufferTextureFormat::RGBA8) },
            { Attachment::Depth, FramebufferTextureSpec(FramebufferTextureFormat::Depth) },
        }
    };

    m_framebuffer = Framebuffer::create(spec);
    m_framebuffer->bind();

    //m_colorBuffer = Texture2D::create(m_width, m_height, GL_RGBA16F);
    //m_depthBuffer = Texture2D::create(m_width, m_height, GL_DEPTH24_STENCIL8);

    //m_framebuffer->attachTexture(*m_colorBuffer, Attachment::Color);
    //m_framebuffer->attachTexture(*m_depthBuffer, Attachment::DepthStencil);

    m_framebuffer->unbind();
}

void RenderTarget::bind() const
{
    RenderCommand::setViewport(0, 0, m_width, m_height);
    m_framebuffer->bind();
}

void RenderTarget::unbind() const
{
    m_framebuffer->unbind();
}

void RenderTarget::resize(uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;

    //invalidate();
    m_framebuffer->resize(width, height);
}

}