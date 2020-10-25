#include <renderer/ui/text/TextMesh.h>

#include <renderer/Shader.h>
#include <util/math/vector/Vector2.h>
#include <renderer/Renderer.h>
#include <renderer/AssetManager.h>

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

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    m_indexBuffer.update(indices, 36);
}

void TextMesh::renderText(const std::string& text, TrueTypeFont& font)
{
    int x = 100;
    int y = 0;
    struct Point
    {
        Vector2f pos;
        Vector2f texCoord;
    };

    Point coords[4 * text.size()];

    m_shader.bind();
    m_shader.setUniform("transform", glm::mat4(1.f));
    m_vertexArray.bind();
    glBindTexture(GL_TEXTURE_2D, font.getTextureAtlas());

    BlendMode::Alpha.bind();

    int n = 0;

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        auto& ch = font.getGlyphs()[*c];

        float x2 = x + ch.pos.x * 2;
        float y2 = -y - ch.pos.y * 2;
        float w = ch.size.x * 2;
        float h = ch.size.y * 2;

        x += ch.advance.x * 2;
        y += ch.advance.y * 2;

        if (!w || !h)
        {
            continue;
        }

        coords[n++] = { Vector2f(x2,     -y2),     Vector2f(ch.texOffset,                                     0) };
        coords[n++] = { Vector2f(x2 + w, -y2),     Vector2f(ch.texOffset + ch.size.x / font.getAtlasSize().x, 0) };
        coords[n++] = { Vector2f(x2 + w, -y2 - h), Vector2f(ch.texOffset + ch.size.x / font.getAtlasSize().x, ch.size.y / font.getAtlasSize().y) };
        coords[n++] = { Vector2f(x2,     -y2 - h), Vector2f(ch.texOffset,                                     ch.size.y / font.getAtlasSize().y) };
    }

    m_vertexBuffer.bind();
    m_vertexBuffer.update(coords, sizeof(coords));
    glDrawElements(GL_TRIANGLES, 6 * text.size(), GL_UNSIGNED_INT, 0);

    glDisable(GL_BLEND);
}