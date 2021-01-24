#include <renderer/RenderTarget.h>
#include <renderer/RenderCommand.h>

namespace Engine
{

RenderTarget::RenderTarget(uint32_t width, uint32_t height)
{
    Framebuffer::Specification spec = {
        width, height,
        {
            { Framebuffer::Attachment::Color, Framebuffer::TextureSpecification(SizedTextureFormat::RGBA8) },
            { Framebuffer::Attachment::Depth, Framebuffer::TextureSpecification(SizedTextureFormat::Depth) },
        }
    };

    m_framebuffer = Framebuffer::create(spec);
}

Shared<RenderTarget> RenderTarget::create(uint32_t width, uint32_t height)
{
    RenderTarget* target = new RenderTarget(width, height);
    return std::shared_ptr<RenderTarget>(target);
}

void RenderTarget::bind() const
{
    m_framebuffer->bind();
}

void RenderTarget::unbind() const
{
    m_framebuffer->unbind();
}

void RenderTarget::resize(uint32_t width, uint32_t height)
{
    m_framebuffer->resize(width, height);
}

}