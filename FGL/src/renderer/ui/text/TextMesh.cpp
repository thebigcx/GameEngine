#include <renderer/ui/text/TextMesh.h>

#include <renderer/Renderer.h>

TextMesh::TextMesh()
{

}

void TextMesh::create()
{
    m_vertexArray.bind();

    BufferLayout layout = {
        { ShaderDataType::Vec2, "aPos" },
        { ShaderDataType::Vec2, "aTexCoord" }
    };

    m_vertexBuffer.create(sizeof(float) * 2 * 2 * 4);
    m_vertexBuffer.setLayout(layout);
    m_vertexArray.addVertexBuffer(m_vertexBuffer);
    m_vertexArray.setIndexBuffer(m_indexBuffer);

    float vertices[] = {
        0.f, 0.f, 0.f, 0.f,
        1.f, 0.f, 1.f, 0.f,
        1.f, 1.f, 1.f, 1.f,
        0.f, 1.f, 0.f, 1.f
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0
    };

    m_vertexBuffer.update(vertices, 16 * sizeof(float));
    m_indexBuffer.update(indices, 6);
}

void TextMesh::renderText(UITextComponent& text)
{
    Renderer::data.fontShader.bind();

    Renderer::data.fontShader.setUniform("textColor", text.getColor());

    m_vertexArray.bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float x = text.getPosition().x;
    float y = text.getPosition().y;

    glm::vec2 scale(text.getSize().x / text.getFont()->getSize(), text.getSize().y / text.getFont()->getSize());

    std::string::const_iterator c;
    for (c = text.getString().begin() ; c != text.getString().end() ; c++)
    {
        Glyph& character = text.getFont()->getCharacters()[*c];

        float xPos = x + character.getBearing().x * scale.x;
        float yPos = y - (character.getSize().y - character.getBearing().y) * scale.y;

        float w = character.getSize().x * scale.x; // scale
        float h = character.getSize().y * scale.y; // scale

        glm::mat4 transform = glm::translate(glm::mat4(1.f), glm::vec3(xPos, yPos, 0.f));
        transform = glm::scale(transform, glm::vec3(w, h, 1.f));

        Renderer::data.fontShader.setUniform("transform", transform);

        glBindTexture(GL_TEXTURE_2D, character.getTexture());

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        x += (character.getAdvance() >> 6) * scale.x; // scale
    }

    glDisable(GL_BLEND);
}