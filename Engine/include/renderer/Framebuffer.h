#pragma once

#include <memory>

#include <core/Core.h>
#include <maths/math.h>

namespace Engine
{

class Framebuffer
{
public:
    virtual ~Framebuffer() = default;

    static Shared<Framebuffer> create(uint32_t width, uint32_t height);

    virtual void resize(uint32_t width, uint32_t height) = 0;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;
    virtual math::vec2 getSize() const = 0;

    virtual uint32_t getColorAttachment() const = 0;
    virtual uint32_t getDepthAttachment() const = 0;

    virtual bool operator==(const Framebuffer& buffer) const = 0;
    virtual bool operator!=(const Framebuffer& buffer) const = 0;
};

}