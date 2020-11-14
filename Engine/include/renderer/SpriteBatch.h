#pragma once

#include <vector>
#include <memory>

#include <renderer/Sprite.h>
#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>
#include <util/OrthographicCamera.h>
#include <renderer/Mesh.h>

class SpriteBatch
{
public:
    SpriteBatch();

    static Shared<SpriteBatch> create(int size = MAX_SPRITES);
    static Shared<SpriteBatch> create(const Shared<Shader>& shader, int size = MAX_SPRITES);

    void setTransformMatrix(const math::Matrix4f& transform);
    void start();
    void renderSprite(const Shared<Texture2D>& texture, const Sprite& quad);
    void renderSprite(const Shared<Texture2D>& texture, const math::Vector2f& position, const math::Vector2f& size);
    void renderSprite(const Shared<Texture2D>& texture, const math::Vector2f& position, const math::Vector2f& size, const FloatRect& texRect);
    void flush();

    void swapTexture(const Shared<Texture2D>& texture);

    // For Renderer class
    inline const Mesh& getMesh() const { return m_mesh; }

private:
    Mesh m_mesh;

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    Shared<Texture2D> m_pLastTexture = nullptr;
    Shared<Shader> m_pShader = nullptr;

    math::Matrix4f m_transform;

    static inline constexpr uint32_t MAX_SPRITES = 100000;
};