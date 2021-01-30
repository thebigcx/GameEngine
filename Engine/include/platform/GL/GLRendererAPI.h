#pragma once

#include <renderer/RendererAPI.h>

namespace Engine
{

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

    void renderIndexed(Reference<VertexArray> array, uint32_t count, uint32_t offset) override;
    void renderInstanced(const Reference<VertexArray>& array, uint32_t instanceCount, uint32_t count, uint32_t offset) override;
};

}