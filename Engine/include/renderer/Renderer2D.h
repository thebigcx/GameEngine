#pragma once

#include <core/Window.h>
#include <util/Color.h>
#include <renderer/shader/Shader.h>
#include <renderer/VertexArray.h>
#include <util/Transform.h>
#include <renderer/text/TrueTypeFont.h>
#include <renderer/Mesh.h>
#include <renderer/IRenderable2D.h>
#include <renderer/Framebuffer.h>
#include <renderer/BlendMode.h>

struct RenderData
{
    uint64_t drawCalls;
    Matrix4f projectionMatrix;
    Shared<Shader> textureShader;
    Shared<Shader> textShader;
    Shared<Shader> framebufferShader;
    BlendMode blendMode = BlendMode::Alpha;
};

class Renderer2D
{
public:
    static void init();

    static void clear();
    static void endFrame();

    static void setClearColor(const Color& color);

    static void render(const Mesh& mesh, const Matrix4f& transform, const Texture2D& texture, Shader& shader);
    static void render(IRenderable2D& renderable);

    static void renderText(const std::string& text, const TrueTypeFont& font, const Vector2f& position, const Color& color = Color(0, 0, 0, 0));
    static void renderText(const std::string& text, const TrueTypeFont& font, const Vector2f& position, const Vector2f& size, const Color& color = Color(0, 0, 0, 0));

    static void renderFramebuffer(const Framebuffer& fbo);

    static RenderData data;

private:
    static Shared<Mesh> m_textMesh;
    static Shared<Mesh> m_framebufferMesh;
        
};