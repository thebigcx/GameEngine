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
    
    ENGINE_ASSERT(batch != nullptr, "Batch creation failed.");

    batch->m_pShader = &shader;
    
    if (size > MAX_SPRITES)
    {
        Console::err("Too many sprites for batch!");
    }

    batch->m_transform = math::Matrix4f();
    batch->m_mesh.vertexArray = VertexArray::create();
    batch->m_mesh.vertexArray->bind();

    BufferLayout layout = {
        { Shader::DataType::Vec2,  "aPos"      },
        { Shader::DataType::Vec2,  "aTexCoord" },
        { Shader::DataType::Color, "aColor"    }
    };

    batch->m_mesh.indexBuffer = IndexBuffer::create(0);

    batch->m_mesh.vertexBuffer = VertexBuffer::create(sizeof(Vertex) * Sprite::getVertexCount() * size);
    batch->m_mesh.vertexBuffer->setLayout(layout);

    batch->m_mesh.vertexArray->addVertexBuffer(batch->m_mesh.vertexBuffer);
    batch->m_mesh.vertexArray->setIndexBuffer(batch->m_mesh.indexBuffer);

    return batch;
}

void SpriteBatch::start()
{
    m_vertices.clear();
    m_indices.clear();
    m_mesh.vertexArray->bind();
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
        auto pos = transform.matrix() * math::Vector4f(Sprite::positions[i].x, Sprite::positions[i].y, 0, 1);
        vertices[i].position = math::Vector2f(pos.x, pos.y);

        // Change "origin" of texCoord, then scale it
        auto rect = sprite.getTextureRect();
        
        // Convert Vector2u to math::Vector2f
        auto texPos = math::Vector2f(rect.x, rect.y) / math::Vector2f(texture.getSize().x, texture.getSize().y);
        auto texSize = math::Vector2f(rect.width, rect.height) / math::Vector2f(texture.getSize().x, texture.getSize().y);

        vertices[i].texCoord = texPos;
        vertices[i].texCoord += Sprite::positions[i] * texSize;

        vertices[i].color = sprite.getColor();
    }

    for (auto& vertex : vertices)
    {
        m_vertices.push_back(vertex);
    }
}

void SpriteBatch::renderSprite(const Texture2D& texture, const math::Vector2f& position, const math::Vector2f& size)
{
    Sprite sprite(position, size, Color(1, 1, 1, 1));
    sprite.setTextureRect(FloatRect(0, 0, texture.getSize().x, texture.getSize().y));
    renderSprite(texture, sprite);
}

void SpriteBatch::renderSprite(const Texture2D& texture, const math::Vector2f& position, const math::Vector2f& size, const FloatRect& texRect)
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

    m_mesh.vertexBuffer->update(&m_vertices[0], sizeof(Vertex) * m_vertices.size());
    m_mesh.indexBuffer->update(&m_indices[0], m_indices.size());

    Renderer2D::render(m_mesh, m_transform, *m_pLastTexture, *m_pShader);
}

void SpriteBatch::setTransformMatrix(const math::Matrix4f& matrix)
{
    m_transform = matrix;
}