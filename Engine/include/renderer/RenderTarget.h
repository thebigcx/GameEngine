#pragma once

#include <core/Core.h>
#include <renderer/Framebuffer.h>

namespace Engine
{

class RenderTarget
{
public:
    static Shared<RenderTarget> create(uint32_t width, uint32_t height);

    void bind() const;
    void unbind() const;

    void resize(uint32_t width, uint32_t height);

    // TODO: make these return Texture2D objects
    inline uint32_t getColorAttachment() const { return m_framebuffer->getColorAttachment(); }
    inline uint32_t getDepthAttachment() const { return m_framebuffer->getDepthAttachment(); }

private:
    RenderTarget(uint32_t width, uint32_t height);

    void invalidate();

    Shared<Framebuffer> m_framebuffer = nullptr;

    //Shared<Texture2D> m_colorBuffer;
    //Shared<Texture2D> m_depthBuffer;
    //Shared<Texture2D> m_stencilBuffer;

    uint32_t m_width = 0, m_height = 0;
};

}