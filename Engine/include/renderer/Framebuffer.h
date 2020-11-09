#pragma once

#include <memory>

class Framebuffer
{
public:
    Framebuffer() {};
    ~Framebuffer();

    static std::shared_ptr<Framebuffer> create(uint32_t width, uint32_t height);

    void resize(uint32_t width, uint32_t height);

    void clear(float r = 0, float g = 0, float b = 0, float a = 0);

    void bind() const;
    void unbind() const;

    inline uint32_t getColorAttachment() const
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