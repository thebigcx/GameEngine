#pragma once

#include <core/Core.h>
#include <renderer/Framebuffer.h>
#include <renderer/Renderer2D.h>
#include <renderer/Renderer3D.h>
#include <renderer/RenderCommand.h>
#include <events/Event.h>

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

    static void startFrame();
    static void endFrame();

    static void windowResize(WindowResizedEvent& event);

private:
    static RendererData m_data;
};