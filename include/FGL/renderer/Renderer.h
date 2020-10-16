#pragma once

#include "../core/Window.h"
#include "../util/Color.h"
#include "Batch.h"
#include "Shader.h"

struct RenderData
{
    Shader shader;
};

class Renderer
{
    public:
        static void init();

        static void clear(const Color& color);
        static void display();

        static void setContext(const Window& window);

        static void renderBatch(const Batch& batch);

    private:
        static const Window* m_currentContext;

        static RenderData m_data;
};