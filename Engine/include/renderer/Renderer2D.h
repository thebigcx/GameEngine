#pragma once

#include <core/Window.h>
#include <renderer/shader/Shader.h>
#include <renderer/VertexArray.h>
#include <util/Transform.h>
#include <renderer/text/TrueTypeFont.h>
#include <renderer/Mesh.h>
#include <renderer/IRenderable2D.h>
#include <renderer/Framebuffer.h>
#include <maths/rect/rect.h>
#include <util/OrthographicCamera.h>
#include <scene/EditorCamera.h>

namespace Engine
{

struct QuadVertex
{
    math::vec3 position;
    math::vec2 texCoord;
    math::vec4 color;
    float texIndex;
};

struct GlyphVertex
{
    math::vec2 position;
    math::vec2 texCoord;
};

struct Renderer2DData
{
    static constexpr uint32_t MAX_SPRITES = 20000;
    static constexpr uint32_t MAX_TEXTURE_SLOTS = 32;
    static constexpr uint32_t MAX_VERTICES = MAX_SPRITES * 4;
    static constexpr uint32_t MAX_INDICES = MAX_SPRITES * 6;
    static constexpr uint32_t MAX_GLYPHS = 512;

    uint64_t drawCalls;

    math::mat4 projectionMatrix;
    Shared<Shader> textureShader;
    Shared<Shader> textShader;

    QuadVertex* vertexBase = nullptr;
    QuadVertex* vertexPointer = nullptr;
    uint32_t indexCount = 0;
    
    Shared<Texture2D> textureSlots[MAX_TEXTURE_SLOTS];
    uint32_t textureSlotIndex = 1;
    
    Mesh mesh;
    
    Shared<Mesh> textMesh;
    GlyphVertex* textVertexBase;
    GlyphVertex* textVertexPtr;

    Shared<UniformBuffer> matrixData;

    static inline const math::vec2 quadPositions[] = {
        math::vec2(0, 0),
        math::vec2(0, 1),
        math::vec2(1, 1),
        math::vec2(1, 0)
    };

    // TODO: look into text batching similar to Renderer3D.
    // At the moment, every string of text is a seperate draw call (inefficient)
};

struct Statistics
{
    uint64_t drawCalls;
};

class Renderer2D
{
public:
    static void startBatch();
    static void nextBatch();
    static void flushBatch();

    static void beginScene(OrthographicCamera& camera);
    static void beginScene(EditorCamera& camera);
    static void beginScene(Camera& camera, const math::mat4& transform);

    static void renderSprite(const Shared<Texture2D>& texture, const math::vec2& position, const math::vec2& size, const math::vec4& color = math::vec4(1));
    static void renderSprite(const Shared<Texture2D>& texture, const math::vec2& position, const math::vec2& size, const math::frect& texRect);
    static void renderSprite(const Shared<Texture2D>& texture, const math::vec2& position, const math::vec2& size, const math::frect& texRect, float rotation, math::vec4 color);
    static void renderSprite(const Shared<Texture2D>& texture, const math::vec2& position, const math::vec2& size, const math::frect& texRect, float rotation, const math::vec2& origin, math::vec4 color);
    static void renderSprite(const Shared<Texture2D>& texture, const math::mat4& transform, const math::frect& texRect, const math::vec4& color = math::vec4(1));
    static void renderSprite(const Shared<Texture2D>& texture, const math::mat4& transform);

    static void renderQuad(const math::vec2& position, const math::vec2& size, const math::vec4& color);
    static void renderQuad(const math::vec2& position, const math::vec2& size, float rotation, const math::vec4& color);
    static void renderQuad(const math::vec2& position, const math::vec2& size, float rotation, const math::vec4& color, const math::vec2& origin);
    static void renderQuad(const math::mat4& transform, const math::vec4& color);
    
    static void endScene();

    static void renderText(const std::string& text, const Shared<TrueTypeFont>& font, const math::vec2& position, const math::vec4& color = math::vec4(0, 0, 0, 0));
    static void renderText(const std::string& text, const Shared<TrueTypeFont>& font, const math::vec2& position, const math::vec2& size, const math::vec4& color = math::vec4(0, 0, 0, 0));

private:
    static Shared<Mesh> m_framebufferMesh;

    static Renderer2DData s_data;
    static Statistics s_statistics;

    static void init();
    static void shutdown();

    friend class Renderer;
};

}