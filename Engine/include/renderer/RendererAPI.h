#pragma once

#include <cstdint>

#include <renderer/VertexArray.h>
#include <core/Core.h>

class RendererAPI
{
public:
    virtual void init() = 0;

    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

    virtual void setClearColor(float r, float g, float b, float a) = 0;
    virtual void clear() = 0;

    virtual void renderIndexed(Shared<VertexArray> array, uint32_t count = 0, uint32_t offset = 0) = 0;

    static Unique<RendererAPI> create();
};