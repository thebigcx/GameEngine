#pragma once

#include <core/Core.h>
#include <renderer/Framebuffer.h>
#include <renderer/Renderer2D.h>
#include <renderer/Renderer3D.h>
#include <renderer/RenderCommand.h>
#include <events/Event.h>
#include <renderer/RenderTarget.h>

namespace Engine
{

struct RendererData
{
    //Reference<Framebuffer> target;
    Reference<RenderTarget> target;
    NonOwning<Shader> fboShader;
    Reference<Mesh> fboMesh;
};

class WindowResizeEvent;

class Renderer
{
public:
    static void init();
    static void shutdown();

    static void startFrame();
    static void endFrame();

    static void windowResize(WindowResizeEvent& event);

    static float hdrExposure;

private:
    static RendererData m_data;
};

}