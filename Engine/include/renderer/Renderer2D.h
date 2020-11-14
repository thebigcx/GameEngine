#pragma once

#include <core/Window.h>
#include <renderer/shader/Shader.h>
#include <renderer/VertexArray.h>
#include <util/Transform.h>
#include <renderer/text/TrueTypeFont.h>
#include <renderer/Mesh.h>
#include <renderer/IRenderable2D.h>
#include <renderer/Framebuffer.h>

struct RenderData
{
    uint64_t drawCalls;
    math::Matrix4f projectionMatrix;
    Shared<Shader> textureShader;
    Shared<Shader> textShader;
    Shared<Shader> framebufferShader;
};

class Renderer2D
{
public:
    static void init();

    static void clear();
    static void endFrame();

    static void setClearColor(const math::Vector4f& color);

    static void render(const Mesh& mesh, const math::Matrix4f& transform, const Shared<Texture2D>& texture, const Shared<Shader>& shader);
    static void render(IRenderable2D& renderable);

    static void renderText(const std::string& text, const TrueTypeFont& font, const math::Vector2f& position, const math::Vector4f& color = math::Vector4f(0, 0, 0, 0));
    static void renderText(const std::string& text, const TrueTypeFont& font, const math::Vector2f& position, const math::Vector2f& size, const math::Vector4f& color = math::Vector4f(0, 0, 0, 0));

    static void renderFramebuffer(const Framebuffer& fbo);

    static RenderData data;

private:
    static Shared<Mesh> m_textMesh;
    static Shared<Mesh> m_framebufferMesh;
        
};