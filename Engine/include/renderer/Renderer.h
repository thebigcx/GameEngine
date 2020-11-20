#pragma once

#include <core/Core.h>
#include <renderer/Framebuffer.h>
#include <renderer/Renderer2D.h>
#include <renderer/Renderer3D.h>
#include <renderer/RenderCommand.h>

struct RendererData
{
    Shared<Framebuffer> target;
    Shared<Shader> fboShader;
    Shared<Mesh> fboMesh;
};

class Renderer
{
public:
    static void init();

    static void setTarget(const Shared<Framebuffer>& target);

    static void startFrame();
    static void endFrame();

private:
    static RendererData m_data;
};