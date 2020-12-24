#pragma once

#include <renderer/RendererAPI.h>

namespace Engine
{

class RenderCommand
{
public:
    static void init()
    {
        m_api = RendererAPI::create();
        m_api->init();
    }

    static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        m_api->setViewport(x, y, width, height);
    }

    static void setClearColor(const math::vec4& color)
    {
        m_api->setClearColor(color);
    }

    static void setDepthTesting(bool enabled)
    {
        m_api->setDepthTesting(enabled);
    }

    static void clear(uint32_t buffer)
    {
        m_api->clear(buffer);
    }

    static void setBlend(bool enabled)
    {
        m_api->setBlend(enabled);
    }

    static void setBlendFunction(BlendFunction src, BlendFunction dst)
    {
        m_api->setBlendFunction(src, dst);
    }

    static void renderIndexed(Shared<VertexArray> array, uint32_t count = 0, uint32_t offset = 0)
    {
        m_api->renderIndexed(array, count, offset);
    }

    static uint32_t defaultClearBits()
    {
        return (uint32_t)RendererBufferType::Color | (uint32_t)RendererBufferType::Depth;
    }

    static RendererCapabilities getCapabilities()
    {
        return m_api->getCapabilities();
    }

private:
    static Unique<RendererAPI> m_api;
};

}