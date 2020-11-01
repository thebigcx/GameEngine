#include <renderer/SpriteBatch.h>
#include <util/Timer.h>
#include <renderer/Renderer2D.h>
#include <renderer/shader/ShaderFactory.h>

#include <GL/glew.h>

#include <iostream>
#include <cstring>

SpriteBatch::SpriteBatch()
{
    
}

Shared<SpriteBatch> SpriteBatch::create(int size)
{
    if (Renderer2D::data.textureShader == nullptr)
    {
        return create(*(ShaderFactory::textureShader()), size);
    }
    return create(*(Renderer2D::data.textureShader), size);
}

Shared<SpriteBatch> SpriteBatch::create(Shader& shader, int size)
{
    auto batch = createShared<SpriteBatch>();
    if (batch == nullptr)
    {
        Console::err("Batch creation failed.");
    }

    batch->m_pShader = &shader;
    
    if (size > MAX_SPRITES)
    {
        Console::err("Too many sprites for batch!");
    }

    batch->m_transform = Matrix4f();
    batch->m_vertexArray.bind();

    BufferLayout layout = {
        { Shader::DataType::Vec2,  "aPos"      },
        { Shader::DataType::Vec2,  "aTexCoord" },
        { Shader::DataType::Color, "aColor"    }
    };

    batch->m_vertexBuffer.setLayout(layout);
    batch->m_vertexBuffer.create(sizeof(Vertex) * Sprite::getVertexCount() * size);

    batch->m_vertexArray.addVertexBuffer(batch->m_vertexBuffer);
    batch->m_vertexArray.setIndexBuffer(batch->m_indexBuf);

    return batch;
}

void SpriteBatch::start()
{
    m_vertices.clear();
    m_indices.clear();
    m_vertexArray.bind();
}

void SpriteBatch::renderSprite(const Texture2D& texture, const Sprite& sprite)
{
    // TODO: figure out why this used to cause segfault
    if (m_pLastTexture == nullptr)
    {
        m_pLastTexture = &texture;
    }
    else if (m_pLastTexture != nullptr)
    {
        if (texture.getId() != m_pLastTexture->getId())
        {
            swapTexture(texture);
        }
    }

    if (m_vertices.size() / Sprite::getVertexCount() > MAX_SPRITES)
    {
        Console::err("Too many sprites for batch!");
    }

    for (int i = 0 ; i < Sprite::indices.size() ; i++)
    {
        m_indices.push_back(Sprite::indices[i] + m_vertices.size());
    }

    auto transform = sprite.getTransform();
    
    std::array<Vertex, 4> vertices;

    // Populate the vertices array with the sprite's vertices
    for (int i = 0 ; i < 4 ; i++)
    {
        auto pos = transform.getMatrix() * Vector4f(Sprite::positions[i].x, Sprite::positions[i].y, 0, 1);
        vertices[i].position = Vector2f(pos.x, pos.y);

        // Change "origin" of texCoord, then scale it
        auto rect = sprite.getTextureRect();
        
        // Convert Vector2u to Vector2f
        auto texPos = Vector2f(rect.x, rect.y) / Vector2f(texture.getSize().x, texture.getSize().y);
        auto texSize = Vector2f(rect.width, rect.height) / Vector2f(texture.getSize().x, texture.getSize().y);

        vertices[i].texCoord = texPos;
        vertices[i].texCoord += Sprite::positions[i] * texSize;

        vertices[i].color = sprite.getColor();
    }

    for (auto& vertex : vertices)
    {
        m_vertices.push_back(vertex);
    }
}

void SpriteBatch::renderSprite(const Texture2D& texture, const Vector2f& position, const Vector2f& size)
{
    Sprite sprite(position, size, Color(1, 1, 1, 1));
    sprite.setTextureRect(FloatRect(0, 0, texture.getSize().x, texture.getSize().y));
    renderSprite(texture, sprite);
}

void SpriteBatch::renderSprite(const Texture2D& texture, const Vector2f& position, const Vector2f& size, const FloatRect& texRect)
{
    Sprite sprite(position, size, Color(1, 1, 1, 1));
    sprite.setTextureRect(texRect);
    renderSprite(texture, sprite);
}

void SpriteBatch::swapTexture(const Texture2D& texture)
{
    flush();
    m_pLastTexture = &texture;
}

void SpriteBatch::flush()
{
    if (m_vertices.size() == 0 || m_pLastTexture == nullptr || m_pShader == nullptr)
    {
        return;
    }

    m_vertexBuffer.update(&m_vertices[0], sizeof(Vertex) * m_vertices.size());
    m_indexBuf.update(&m_indices[0], m_indices.size());

    Renderer2D::renderArray(m_vertexArray, m_transform, *m_pLastTexture, *m_pShader);
}

void SpriteBatch::setTransformMatrix(const Matrix4f& matrix)
{
    m_transform = matrix;
}