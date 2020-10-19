#pragma once

#include "../core/Window.h"
#include "../util/Color.h"
#include "Batch.h"
#include "Shader.h"

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

        static void renderBatch(const Batch& batch);

        static void renderIndexed(const VertexArray& array, RenderStates states);

    private:
        static RenderData m_data;
};