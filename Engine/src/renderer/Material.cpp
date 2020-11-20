#include <renderer/Material.h>

void Material::setTexture(const Shared<Texture2D>& texture)
{
    //m_textures.push_back(texture);
    m_texture = texture;
}

void Material::bind() const
{
    m_shader->bind();
    //m_textures[0]->bind(); // TODO: multiple textures
    m_texture->bind();
}

void Material::unbind() const
{
    m_shader->unbind();
    //m_textures[0]->unbind();
    m_texture->bind();
}

Shared<Material> Material::create(const Shared<Shader>& shader)
{
    auto material = createShared<Material>();

    material->m_shader = shader;

    return material;
}