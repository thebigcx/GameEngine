#include <renderer/Renderer2D.h>
#include <core/Application.h>
#include <renderer/MeshFactory.h>
#include <renderer/shader/ShaderFactory.h>

#include <GL/glew.h>

RenderData Renderer2D::data;
Shared<Mesh> Renderer2D::m_textMesh;

void Renderer2D::init()
{
    glDisable(GL_DEPTH_TEST);

    auto size = Application::get().getWindow().getSize();

    data.projectionMatrix = Matrix4f::createOrthoProjection(0.f, size.x, 0.f, size.y, -1.f, 1.f);

    data.textureShader = ShaderFactory::textureShader();
    data.textureShader->bind();
    data.textureShader->setUniform("projection", data.projectionMatrix);

    data.textShader = ShaderFactory::textShader();
    data.textShader->bind();
    data.textShader->setUniform("projection", data.projectionMatrix);

    m_textMesh = MeshFactory::textMesh();
}

void Renderer2D::startFrame()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer2D::endFrame()
{
    auto& window = Application::get().getWindow();
    glfwSwapBuffers(window.getNative());
}

void Renderer2D::setClearColor(const Color& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer2D::renderArray(const VertexArray& array, RenderStates states)
{
    states.bind();
    states.shader->setUniform("transform", states.transform);
    array.bind();

    glDrawElements(GL_TRIANGLES, array.getIndexBuffer()->getCount(), array.getIndexBuffer()->getIndexType(), 0);
    data.drawCalls++;
}

void Renderer2D::renderArray(const VertexArray& array, const Transform& transform, const Texture2D& texture)
{
    renderArray(array, transform.matrix(), texture);
}

void Renderer2D::renderArray(const VertexArray& array, const Matrix4f& transform, const Texture2D& texture)
{
    renderArray(array, transform, texture, *data.textureShader);
}

void Renderer2D::renderArray(const VertexArray& array, const Matrix4f& transform, const Texture2D& texture, Shader& shader)
{
    shader.bind();
    shader.setUniform("transform", transform);
    BlendMode::Alpha.bind();
    texture.bind();
    array.bind();

    glDrawElements(GL_TRIANGLES, array.getIndexBuffer()->getCount(), array.getIndexBuffer()->getIndexType(), 0);
    data.drawCalls++;
}

void Renderer2D::renderText(const std::string& text, const TrueTypeFont& font, const Vector2f& position, const Color& color)
{
    renderText(text, font, position, Vector2f(font.getCharacterSize()), color);
}

void Renderer2D::renderText(const std::string& text, const TrueTypeFont& font, const Vector2f& position, const Vector2f& size, const Color& color)
{
    struct GlyphVertex
    {
        Vector2f position;
        Vector2f texCoord;
    };

    Vector2f scale = size / font.getCharacterSize();
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
    m_textMesh->indexBuffer.update(&indices[0], indices.size());

    int x = position.x;
    int y = position.y;

    GlyphVertex coords[4 * text.size()];

    // Set render states
    data.textShader->bind();
    data.textShader->setUniform("transform", Matrix4f(1.f));
    data.textShader->setUniform("textColor", color);
    m_textMesh->vertexArray.bind();
    font.getTextureAtlas()->bind();

    BlendMode::Alpha.bind();

    int n = 0;

    // Loop through each character in the string, and add its texCoords to the mesh
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        auto& ch = font.getGlyphs().at(*c);

        Vector2f pos(x + ch.pos.x * scale.x, -y - ch.pos.y * scale.y);
        Vector2f size = ch.size * scale;

        x += ch.advance.x * scale.x;
        y += ch.advance.y * scale.y;

        if (!size.x || !size.y)
            continue;

        coords[n++] = { Vector2f(pos.x,          -pos.y),          Vector2f(ch.texOffset,                                     0) };
        coords[n++] = { Vector2f(pos.x + size.x, -pos.y),          Vector2f(ch.texOffset + ch.size.x / font.getAtlasSize().x, 0) };
        coords[n++] = { Vector2f(pos.x + size.x, -pos.y - size.y), Vector2f(ch.texOffset + ch.size.x / font.getAtlasSize().x, ch.size.y / font.getAtlasSize().y) };
        coords[n++] = { Vector2f(pos.x,          -pos.y - size.y), Vector2f(ch.texOffset,                                     ch.size.y / font.getAtlasSize().y) };
    }

    // Draw the text mesh
    m_textMesh->vertexBuffer.bind();
    m_textMesh->vertexBuffer.update(coords, sizeof(coords));
    glDrawElements(GL_TRIANGLES, 6 * text.size(), GL_UNSIGNED_INT, 0);

    BlendMode::Alpha.unbind();
}

void Renderer2D::render(IRenderable2D& renderable)
{
    renderable.render();
}