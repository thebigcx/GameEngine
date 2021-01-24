#pragma once

#include <cstdint>
#include <unordered_map>

#include <renderer/Framebuffer.h>
#include <maths/math.h>
#include <platform/GL/GLTexture2D.h>

namespace Engine
{

class GLRenderbuffer : public Renderbuffer
{
public:
    GLRenderbuffer(uint32_t width, uint32_t height, GLenum internalFormat);
    ~GLRenderbuffer();

    void bind() const override;
    void unbind() const override;

    inline uint32_t getWidth() const override { return m_width; }
    inline uint32_t getHeight() const override { return m_height; }

    inline uint32_t getId() const { return m_id; }

private:
    uint32_t m_id = 0;
    uint32_t m_width = 0, m_height = 0;
};

class GLFramebuffer : public Framebuffer
{
public:
    GLFramebuffer();
    GLFramebuffer(const Specification& spec);
    GLFramebuffer(uint32_t width, uint32_t height);
    ~GLFramebuffer();

    void resize(uint32_t width, uint32_t height) override;

    void bind() const override;
    void unbind() const override;

    inline uint32_t getWidth() const override { return m_width; }
    inline uint32_t getHeight() const override { return m_height; }
    inline math::vec2 getSize() const override { return math::vec2(m_width, m_height); }

    inline uint32_t getColorAttachment(uint32_t index = 0) const override { return m_colorAttachments[index]; }
    inline uint32_t getDepthAttachment() const override { return m_depthAttachment; }

    void attachRenderbuffer(const Renderbuffer& buffer, Attachment attachment) override;
    void attachTexture(const Texture2D& texture, Attachment attachment, GLenum target) override;

    void drawBuffer(uint32_t buffer) const override;
    void readBuffer(uint32_t buffer) const override;

    inline bool operator==(const Framebuffer& buffer) const override
    {
        return static_cast<const GLFramebuffer&>(buffer).m_id == m_id;
    }

    inline bool operator!=(const Framebuffer& buffer) const override
    {
        return static_cast<const GLFramebuffer&>(buffer).m_id != m_id;
    }

private:
    uint32_t m_id = 0;

    std::vector<uint32_t> m_colorAttachments;
    uint32_t m_depthAttachment = 0;

    std::vector<TextureSpecification> m_colorAttachmentSpecs;
    TextureSpecification m_depthAttachmentSpec;

    uint32_t m_linearFiltering = true;

    uint32_t m_width = 0, m_height = 0;

    void invalidate();

    static GLenum getColorBufferEnumValue_(uint32_t buffer);

    static GLenum getAttachmentEnumValue_(Attachment attachment);

    static GLenum getTextureFormatEnumValue_(SizedTextureFormat format);

    static constexpr int s_maxSize = 8192;
};

}