#pragma once

#include <core/Core.h>
#include <renderer/Framebuffer.h>

namespace Engine
{

/**
 * A simple wrapper over Framebuffer that provides the defaults for 
 * a render target.
 */
class RenderTarget
{
public:
    static Reference<RenderTarget> create(uint32_t width, uint32_t height);

    void bind() const;
    void unbind() const;

    void resize(uint32_t width, uint32_t height);

    // TODO: make these return Texture2D objects
    inline uint32_t getColorAttachment() const { return m_framebuffer->getColorAttachment(); }
    inline uint32_t getDepthAttachment() const { return m_framebuffer->getDepthAttachment(); }

private:
    RenderTarget(uint32_t width, uint32_t height);

    Reference<Framebuffer> m_framebuffer = nullptr;
};

}