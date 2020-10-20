#pragma once

#include <core/Window.h>
#include <util/Color.h>
#include <renderer/Shader.h>
#include <renderer/VertexArray.h>
#include <renderer/RenderStates.h>

struct RenderData
{
    uint64_t drawCalls;
};

class Renderer
{
    public:
        static void init();

        static void startRender();
        static void finishRender();

        static void setClearColor(const Color& color);

        static void renderIndexed(const VertexArray& array, RenderStates states);

    private:
        static RenderData m_data;
};