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
#include <maths/maths/rect.h>

struct Renderer2DData
{
    uint64_t drawCalls;

    math::mat4 projectionMatrix;
    Shared<Shader> textureShader;
    Shared<Shader> textShader;
    Shared<Texture2D> whiteTexture;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    Shared<Texture2D> activeTexture = nullptr;
    math::mat4 transform;
    Mesh mesh;

    static inline const math::vec2 quadPositions[] = {
        math::vec2(0, 0),
        math::vec2(0, 1),
        math::vec2(1, 1),
        math::vec2(1, 0)
    };

    const int MAX_SPRITES = 100000;
};

struct Statistics
{
    uint64_t drawCalls;
};

class Renderer2D
{
public:
    static void init();

    static void render(IRenderable2D& renderable);

    static void startBatch(const math::mat4& transform = math::mat4(1.f));

    static void renderSprite(const Shared<Texture2D>& texture, const math::vec2& position, const math::vec2& size);
    static void renderSprite(const Shared<Texture2D>& texture, const math::vec2& position, const math::vec2& size, const math::vec4& color);
    static void renderSprite(const Shared<Texture2D>& texture, const math::vec2& position, const math::vec2& size, const math::frect& texRect);
    static void renderSprite(const Shared<Texture2D>& texture, const math::vec2& position, const math::vec2& size, const math::frect& texRect, float rotation, math::vec4 color);
    static void renderSprite(const Shared<Texture2D>& texture, const math::vec2& position, const math::vec2& size, const math::frect& texRect, float rotation, const math::vec2& origin, math::vec4 color);
    
    static void renderQuad(const math::vec2& position, const math::vec2& size, const math::vec4& color);
    static void renderQuad(const math::vec2& position, const math::vec2& size, float rotation, const math::vec4& color);
    
    static void endBatch();
    static void flushBatch();

    static void renderText(const std::string& text, const Shared<TrueTypeFont>& font, const math::vec2& position, const math::vec4& color = math::vec4(0, 0, 0, 0));
    static void renderText(const std::string& text, const Shared<TrueTypeFont>& font, const math::vec2& position, const math::vec2& size, const math::vec4& color = math::vec4(0, 0, 0, 0));

    //static Statistics stats;

private:
    static Shared<Mesh> m_textMesh;
    static Shared<Mesh> m_framebufferMesh;
public:
    static Renderer2DData data;
};