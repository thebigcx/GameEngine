#include <renderer/Material.h>

void Material::setTexture(const Shared<Texture2D>& texture)
{
    m_textures.push_back(texture);
}

void Material::bind() const
{
    m_shader->bind();
    for (unsigned int i = 0; i < m_textures.size(); i++)
    {
        m_textures[i]->bind(i);
    }
}

void Material::unbind() const
{
    m_shader->unbind();
    for (unsigned int i = 0; i < m_textures.size(); i++)
    {
        m_textures[i]->unbind(i);
    }
}

Shared<Material> Material::create(const Shared<Shader>& shader)
{
    auto material = createShared<Material>();

    material->m_shader = shader;

    return material;
}