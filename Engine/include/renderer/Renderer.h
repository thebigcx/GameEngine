#pragma once

#include <core/Window.h>
#include <util/Color.h>
#include <renderer/Shader.h>
#include <renderer/VertexArray.h>
#include <renderer/RenderStates.h>
#include <util/Transform.h>
#include <renderer/text/TextMesh.h>

struct RenderData
{
    uint64_t drawCalls;
    glm::mat4 projectionMatrix;
    Shader textureShader;
    BlendMode blendMode = BlendMode::Alpha;
};

class Renderer
{
public:
    static void init();

    static void startFrame();
    static void endFrame();

    static void setClearColor(const Color& color);

    static void renderArray(const VertexArray& array, RenderStates states);
    static void renderArray(const VertexArray& array, const Transform& transform, const Texture2D& texture);
    static void renderArray(const VertexArray& array, const glm::mat4& transform, const Texture2D& texture);
    static void renderArray(const VertexArray& array, const glm::mat4& transform, const Texture2D& texture, Shader& shader);

    static void renderText(const std::string& text, const TrueTypeFont& font, const Vector2f& position, const Color& color = Color(0, 0, 0, 0));
    static void renderText(const std::string& text, const TrueTypeFont& font, const Vector2f& position, const Vector2f& size, const Color& color = Color(0, 0, 0, 0));

    static RenderData data;

private:
    static Unique<TextMesh> m_textMesh;
        
};