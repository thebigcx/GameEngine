#include <renderer/text/TextMesh.h>

#include <renderer/Shader.h>
#include <util/math/vector/Vector2.h>
#include <util/math/vector/Vector4.h>
#include <renderer/Renderer.h>

Shader TextMesh::m_shader;

TextMesh::TextMesh()
{

}

void TextMesh::create()
{
    m_shader.create("shaders/text.vert", "shaders/text.frag");
    m_shader.bind();
    m_shader.setUniform("projection", Renderer::data.projectionMatrix);

    m_vertexArray.bind();

    BufferLayout layout = {
        { Shader::DataType::Vec2, "aPos" },
        { Shader::DataType::Vec2, "aTexCoord" }
    };

    m_vertexBuffer.create(sizeof(float) * 1000);
    m_vertexBuffer.setLayout(layout);
    m_vertexArray.addVertexBuffer(m_vertexBuffer);
    m_vertexArray.setIndexBuffer(m_indexBuffer);
}

void TextMesh::renderText(const std::string& text, const TrueTypeFont& font, const Color& color, const Vector2f& size)
{
    Vector2f scale = size / TrueTypeFont::FONT_SIZE;
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
    m_indexBuffer.update(&indices[0], indices.size());


    int x = 100;
    int y = 0;

    GlyphVertex coords[4 * text.size()];

    m_shader.bind();
    m_shader.setUniform("transform", glm::mat4(1.f));
    m_shader.setUniform("textColor", color);
    m_vertexArray.bind();
    glBindTexture(GL_TEXTURE_2D, font.getTextureAtlas());

    BlendMode::Alpha.bind();

    int n = 0;

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        auto& ch = font.getGlyphs().at(*c);

        Vector2f pos(x + ch.pos.x * scale.x, -y - ch.pos.y * scale.y);
        Vector2f size(ch.size.x * scale.x, ch.size.y * scale.y);

        x += ch.advance.x * scale.x;
        y += ch.advance.y * scale.y;

        if (!size.x || !size.y)
        {
            continue;
        }

        coords[n++] = { Vector2f(pos.x,          -pos.y),          Vector2f(ch.texOffset,                                     0) };
        coords[n++] = { Vector2f(pos.x + size.x, -pos.y),          Vector2f(ch.texOffset + ch.size.x / font.getAtlasSize().x, 0) };
        coords[n++] = { Vector2f(pos.x + size.x, -pos.y - size.y), Vector2f(ch.texOffset + ch.size.x / font.getAtlasSize().x, ch.size.y / font.getAtlasSize().y) };
        coords[n++] = { Vector2f(pos.x,          -pos.y - size.y), Vector2f(ch.texOffset,                                     ch.size.y / font.getAtlasSize().y) };
    }

    m_vertexBuffer.bind();
    m_vertexBuffer.update(coords, sizeof(coords));
    glDrawElements(GL_TRIANGLES, 6 * text.size(), GL_UNSIGNED_INT, 0);

    glDisable(GL_BLEND);
}

void TextMesh::renderText(const Text& text)
{
    renderText(text.getString(), *text.getFont(), text.getColor(), text.getCharSize());
}