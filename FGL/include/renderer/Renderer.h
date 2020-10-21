#pragma once

#include <core/Window.h>
#include <util/Color.h>
#include <renderer/Shader.h>
#include <renderer/VertexArray.h>
#include <renderer/RenderStates.h>

struct RenderData
{
    uint64_t drawCalls;
    glm::mat4 projectionMatrix;
    Shader textureShader;
};

class Renderer
{
    public:
        static void init();

        static void startFrame();
        static void endFrame();

        static void setClearColor(const Color& color);

        static void render(const VertexArray& array, RenderStates states);
        static void render(const VertexArray& array, const glm::mat4& transform, const Texture& texture);

    private:
        static RenderData m_data;
};