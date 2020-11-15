#pragma once

#include <core/Window.h>
#include <renderer/shader/Shader.h>
#include <renderer/VertexArray.h>
#include <util/Transform.h>
#include <renderer/text/TrueTypeFont.h>
#include <renderer/Mesh.h>
#include <renderer/IRenderable2D.h>
#include <renderer/Framebuffer.h>
#include <renderer/Vertex.h>
#include <math/Rect.h>

struct RenderData
{
    uint64_t drawCalls;
    math::mat4 projectionMatrix;
    Shared<Shader> textureShader;
    Shared<Shader> textShader;
    Shared<Shader> framebufferShader;
    Shared<Texture2D> whiteTexture;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    Shared<Texture2D> activeTexture = nullptr;
    math::mat4 transform;
    Mesh mesh;

    const int MAX_SPRITES = 100000;
};

class Renderer2D
{
public:
    static void init();

    static void clear();
    static void endFrame();

    static void setClearColor(const math::vec4& color);

    static void render(const Mesh& mesh, const math::mat4& transform, const Shared<Texture2D>& texture, const Shared<Shader>& shader);
    static void render(IRenderable2D& renderable);

    static void startBatch();
    static void renderSprite(const Shared<Texture2D>& texture, const math::vec2& position, const math::vec2& size, const FloatRect& texRect, float rotation, math::vec4 color);
    static void renderQuad(const math::vec2& position, const math::vec2& size, float rotation, math::vec4 color);
    static void endBatch();
    static void flushBatch();

    static void renderText(const std::string& text, const TrueTypeFont& font, const math::vec2& position, const math::vec4& color = math::vec4(0, 0, 0, 0));
    static void renderText(const std::string& text, const TrueTypeFont& font, const math::vec2& position, const math::vec2& size, const math::vec4& color = math::vec4(0, 0, 0, 0));

    static void renderFramebuffer(const Framebuffer& fbo);

    static RenderData data;

private:
    static Shared<Mesh> m_textMesh;
    static Shared<Mesh> m_framebufferMesh;
        
};