#pragma once

#include <cstdint>

#include <renderer/VertexArray.h>
#include <core/Core.h>

enum class BlendFunction
{
    None,
    Zero,
    One,
    SourceAlpha,
    DestinationAlpha,
    OneMinusSourceAlpha
};

enum class RendererBufferType
{
    None    = 0,
    Color   = 1 << 0,
    Depth   = 1 << 1,
    Stencil = 1 << 2
};

class RendererAPI
{
public:
    virtual void init() = 0;

    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

    virtual void setDepthTesting(bool enabled) = 0;
    virtual void setBlend(bool enabled) = 0;
    virtual void setBlendFunction(BlendFunction src, BlendFunction dst) = 0;

    virtual void setClearColor(float r, float g, float b, float a) = 0;
    virtual void clear(uint32_t buffer) = 0;

    virtual void renderIndexed(Shared<VertexArray> array, uint32_t count, uint32_t offset) = 0;

    static Unique<RendererAPI> create();
};