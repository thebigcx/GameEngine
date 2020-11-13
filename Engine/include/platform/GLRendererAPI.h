#pragma once

#include <renderer/RendererAPI.h>

class GLRendererAPI : public RendererAPI
{
public:
    void init() override;

    void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

    void setClearColor(float r, float g, float b, float a) override;
    void clear() override;

    void renderIndexed(Shared<VertexArray> array) override;
};