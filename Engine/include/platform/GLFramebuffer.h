#pragma once

#include <cstdint>

#include <renderer/Framebuffer.h>

class GLFramebuffer : public Framebuffer
{
public:
    GLFramebuffer(uint32_t width, uint32_t height);
    ~GLFramebuffer();

    void resize(uint32_t width, uint32_t height) override;

    void bind() const override;
    void unbind() const override;

    inline uint32_t getColorAttachment() const override
    {
        return m_colorAttachment;
    }

private:
    uint32_t m_id = 0;
    uint32_t m_colorAttachment = 0;
    uint32_t m_depthAttachment = 0;

    uint32_t m_width, m_height;

    void invalidate(uint32_t width, uint32_t height);

    static constexpr int s_maxSize = 8192;
};