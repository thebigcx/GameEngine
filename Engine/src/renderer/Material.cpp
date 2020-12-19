#include <renderer/Material.h>

void Material::bind() const
{
    shader->bind();

    if (usingAlbedoMap)
    {
        albedoMap->bind(0);
        shader->setInt("material.albedo", 0);
    }

    if (usingNormalMap)
    {
        normalMap->bind(1);
        shader->setInt("material.normal", 1);
        shader->setInt("material.usingNormal", 1);
    }
    else
    {
        shader->setInt("material.usingNormal", 0);
    }

    if (usingMetalnessMap)
    {
        metalnessMap->bind(2);
        shader->setInt("material.metallic", 2);
    }

    if (usingRoughnessMap)
    {
        roughnessMap->bind(3);
        shader->setInt("material.roughness", 3);
    }

    if (usingAmbientOcclusionMap)
    {
        ambientOcclusionMap->bind(4);
        shader->setInt("material.ao", 4);
    }
}

void Material::unbind() const
{
    shader->unbind();

    albedoMap->unbind(0);
    normalMap->unbind(1);
    metalnessMap->unbind(2);
    roughnessMap->unbind(3);
}

Shared<Material> Material::create(const Shared<Shader>& shader)
{
    auto material = createShared<Material>();

    material->shader = shader;

    uint32_t white = 0xffffffff;
    uint32_t black = 0x00000000;
    
    material->albedoMap = Texture2D::create(1, 1);
    material->albedoMap->setData(0, 0, 1, 1, &white);

    material->normalMap = Texture2D::create(1, 1);
    material->normalMap->setData(0, 0, 1, 1, &black);

    material->metalnessMap = Texture2D::create(1, 1);
    material->metalnessMap->setData(0, 0, 1, 1, &white);

    material->roughnessMap = Texture2D::create(1, 1);
    material->roughnessMap->setData(0, 0, 1, 1, &white);

    material->ambientOcclusionMap = Texture2D::create(1, 1);
    material->ambientOcclusionMap->setData(0, 0, 1, 1, &white);

    material->albedoColor = math::vec4(1, 1, 1, 1);

    return material;
}