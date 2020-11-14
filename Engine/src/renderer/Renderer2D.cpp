#include <renderer/Renderer2D.h>
#include <core/Application.h>
#include <renderer/MeshFactory.h>
#include <renderer/shader/ShaderFactory.h>
#include <math/matrix/matrix_transform.h>
#include <renderer/RenderCommand.h>

#include <GL/glew.h>

RenderData Renderer2D::data;
Shared<Mesh> Renderer2D::m_textMesh;
Shared<Mesh> Renderer2D::m_framebufferMesh;

void Renderer2D::init()
{
    glDisable(GL_DEPTH_TEST);

    auto size = Application::get().getWindow().getSize();

    data.projectionMatrix = math::ortho(0.f, (float)size.x, 0.f, (float)size.y, -1.f, 1.f);

    data.textureShader = ShaderFactory::textureShader();
    data.textureShader->bind();
    data.textureShader->setMatrix4("projection", data.projectionMatrix);

    data.textShader = ShaderFactory::textShader();
    data.textShader->bind();
    data.textShader->setMatrix4("projection", data.projectionMatrix);

    data.framebufferShader = ShaderFactory::framebufferShader();

    m_textMesh = MeshFactory::textMesh();
    m_framebufferMesh = MeshFactory::quadMesh(-1, -1, 1, 1); // Normalized device coordinates
}

void Renderer2D::clear()
{
    RenderCommand::clear();
}

void Renderer2D::endFrame()
{
    
}

void Renderer2D::setClearColor(const math::vec4& color)
{
    RenderCommand::setClearColor(color.r, color.g, color.b, color.a);
}

void Renderer2D::render(const Mesh& mesh, const math::mat4& transform, const Shared<Texture2D>& texture, const Shared<Shader>& shader)
{
    shader->bind();
    shader->setMatrix4("transform", transform);
    texture->bind();

    mesh.vertexArray->bind();

    RenderCommand::renderIndexed(mesh.vertexArray);
    data.drawCalls++;
}

void Renderer2D::renderText(const std::string& text, const TrueTypeFont& font, const math::vec2& position, const math::vec4& color)
{
    renderText(text, font, position, math::vec2(font.getCharacterSize()), color);
}

void Renderer2D::renderText(const std::string& text, const TrueTypeFont& font, const math::vec2& position, const math::vec2& size, const math::vec4& color)
{
    struct GlyphVertex
    {
        math::vec2 position;
        math::vec2 texCoord;
    };

    math::vec2 scale = size / font.getCharacterSize();
    std::vector<unsigned int> indices;

    // Set the indices
    for (int i = 0 ; i < text.size() ; i++)
    {
        indices.push_back(0 + i * 4);
        indices.push_back(1 + i * 4);
        indices.push_back(2 + i * 4);
        indices.push_back(2 + i * 4);
        indices.push_back(3 + i * 4);
        indices.push_back(0 + i * 4);
    }
    m_textMesh->indexBuffer->update(&indices[0], indices.size());

    int x = position.x;
    int y = position.y;

    GlyphVertex coords[4 * text.size()];

    // Set render states
    data.textShader->bind();
    data.textShader->setMatrix4("transform", math::identity<float>());
    data.textShader->setFloat4("textColor", math::vec4(color.r, color.g, color.b, color.a));
    m_textMesh->vertexArray->bind();
    font.getTextureAtlas()->bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int n = 0;

    // Loop through each character in the string, and add its texCoords to the mesh
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        auto& ch = font.getGlyphs().at(*c);

        math::vec2 pos(x + ch.pos.x * scale.x, -y - ch.pos.y * scale.y);
        math::vec2 size = ch.size * scale;

        x += ch.advance.x * scale.x;
        y += ch.advance.y * scale.y;

        if (!size.x || !size.y)
            continue;

        coords[n++] = { math::vec2(pos.x,          -pos.y),          math::vec2(ch.texOffset,                                     0) };
        coords[n++] = { math::vec2(pos.x + size.x, -pos.y),          math::vec2(ch.texOffset + ch.size.x / font.getAtlasSize().x, 0) };
        coords[n++] = { math::vec2(pos.x + size.x, -pos.y - size.y), math::vec2(ch.texOffset + ch.size.x / font.getAtlasSize().x, ch.size.y / font.getAtlasSize().y) };
        coords[n++] = { math::vec2(pos.x,          -pos.y - size.y), math::vec2(ch.texOffset,                                     ch.size.y / font.getAtlasSize().y) };
    }

    // Draw the text mesh
    m_textMesh->vertexBuffer->bind();
    m_textMesh->vertexBuffer->update(coords, sizeof(coords));
    RenderCommand::renderIndexed(m_textMesh->vertexArray);

    glDisable(GL_BLEND);
}

void Renderer2D::renderFramebuffer(const Framebuffer& fbo)
{
    glBindTextureUnit(0, fbo.getColorAttachment());
    data.framebufferShader->bind();

    m_framebufferMesh->vertexArray->bind();
    RenderCommand::renderIndexed(m_framebufferMesh->vertexArray);
}

void Renderer2D::render(IRenderable2D& renderable)
{
    renderable.render();
}

