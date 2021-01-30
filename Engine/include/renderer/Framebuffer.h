#pragma once

#include <memory>

#include <GL/glew.h>

#include <core/Core.h>
#include <maths/math.h>
#include <renderer/Texture2D.h>

namespace Engine
{

enum class ColorBuffer
{
    None = 0,
    FrontLeft = 1 << 0,
    FrontRight = 1 << 1,
    BackLeft = 1 << 2,
    BackRight = 1 << 3
};

class Renderbuffer
{
public:
    virtual ~Renderbuffer() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;

    virtual uint32_t getId() const = 0;

    static Reference<Renderbuffer> create(uint32_t width, uint32_t height, GLenum internalFormat);
};

class Framebuffer
{
public:

    enum class Attachment
    {
        Color, Depth, Stencil, DepthStencil
    };

    struct TextureSpecification
    {
        TextureSpecification() = default;
        TextureSpecification(SizedTextureFormat format_)
            : format(format_) {}
        
        SizedTextureFormat format = SizedTextureFormat::None;
    };

    struct AttachmentSpecification
    {
        Attachment attachment;
        TextureSpecification texture;
    };

    struct Specification
    {
        uint32_t width = 0, height = 0;
        std::vector<AttachmentSpecification> attachments;
    };

public:
    virtual ~Framebuffer() = default;

    static Reference<Framebuffer> create();
    static Reference<Framebuffer> create(uint32_t width, uint32_t height);
    static Reference<Framebuffer> create(const Specification& spec);

    virtual void resize(uint32_t width, uint32_t height) = 0;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;
    virtual math::vec2 getSize() const = 0;

    virtual uint32_t getColorAttachment(uint32_t index = 0) const = 0;
    virtual uint32_t getDepthAttachment() const = 0;

    virtual void attachRenderbuffer(const Renderbuffer& buffer, Attachment attachment) = 0;
    virtual void attachTexture(const Texture2D& texture, Attachment attachment, GLenum target = GL_TEXTURE_2D) = 0;

    virtual void drawBuffer(uint32_t buffer) const = 0;
    virtual void readBuffer(uint32_t buffer) const = 0;

    virtual bool operator==(const Framebuffer& buffer) const = 0;
    virtual bool operator!=(const Framebuffer& buffer) const = 0;

    static const Framebuffer* getCurrentBoundFramebuffer()
    {
        return s_currentBound;
    }

protected:
    static inline const Framebuffer* s_currentBound = nullptr;
};

}