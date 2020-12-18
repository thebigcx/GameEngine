#include <renderer/Material.h>

void Material::bind() const
{
    shader->bind();

    if (usingAlbedoMap)
    {
        albedoMap->bind(0);
        shader->setFloat4("material.albedoColor", albedoColor);
    }

    if (usingNormalMap)
    {
        normalMap->bind(1);
    }

    if (usingMetalnessMap)
    {
        metalnessMap->bind(2);
    }

    if (usingRoughnessMap)
    {
        roughnessMap->bind(3);
    }
}

void Material::unbind() const
{
    shader->unbind();

    if (usingAlbedoMap)
    {
        albedoMap->unbind(0);
    }

    if (usingNormalMap)
    {
        normalMap->unbind(1);
    }

    if (usingMetalnessMap)
    {
        metalnessMap->unbind(2);
    }   

    if (usingRoughnessMap)
    {
        roughnessMap->unbind(3);
    }
}

Shared<Material> Material::create(const Shared<Shader>& shader)
{
    auto material = createShared<Material>();

    material->shader = shader;

    uint32_t white = 0xffffffff;
    
    material->albedoMap = Texture2D::create(1, 1);
    material->albedoMap->setData(0, 0, 1, 1, &white);

    material->normalMap = Texture2D::create(1, 1);
    material->normalMap->setData(0, 0, 1, 1, &white);

    material->metalnessMap = Texture2D::create(1, 1);
    material->metalnessMap->setData(0, 0, 1, 1, &white);

    material->roughnessMap = Texture2D::create(1, 1);
    material->roughnessMap->setData(0, 0, 1, 1, &white);

    material->albedoColor = math::vec4(1, 1, 1, 1);

    return material;
}