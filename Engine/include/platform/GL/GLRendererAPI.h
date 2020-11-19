#pragma once

#include <renderer/RendererAPI.h>

class GLRendererAPI : public RendererAPI
{
public:
    void init() override;

    void setDepthTesting(bool enabled) override;
    void setBlend(bool enabled) override;
    void setBlendFunction(BlendFunction src, BlendFunction dst) override;

    void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

    void setClearColor(const math::vec4& color) override;
    void clear(uint32_t buffer) override;

    void renderIndexed(Shared<VertexArray> array, uint32_t count = 0, uint32_t offset = 0) override;
};