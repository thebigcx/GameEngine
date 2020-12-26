#include <renderer/Material.h>
#include <renderer/Renderer3D.h>

namespace Engine
{

void Material::bind() const
{
    if (shader)
    {
        shader->bind();
    }
    else
    {
        return;
    }

    if (usingAlbedoMap && albedoMap)
    {
        albedoMap->bind(0);
        shader->setInt("material.albedo", 0);
        shader->setInt("material.usingAlbedo", 1);
    }
    else
    {
        shader->setInt("material.usingAlbedo", 0);
        shader->setFloat3("material.albedoColor", albedoColor);
    }

    if (usingNormalMap && normalMap)
    {
        normalMap->bind(1);
        shader->setInt("material.normal", 1);
        shader->setInt("material.usingNormal", 1);
    }
    else
    {
        shader->setInt("material.usingNormal", 0);
    }

    if (usingMetallicMap && metallicMap)
    {
        metallicMap->bind(2);
        shader->setInt("material.metallic", 2);
        shader->setInt("material.usingMetalness", 1);
    }
    else
    {
        shader->setInt("material.usingMetalness", 0);
    }
    shader->setFloat("material.metallicScalar", metallicScalar);
    

    if (usingRoughnessMap && roughnessMap)
    {
        roughnessMap->bind(3);
        shader->setInt("material.roughness", 3);
        shader->setInt("material.usingRoughness", 1);
    }
    else
    {
        shader->setInt("material.usingRoughness", 0);
    }
    shader->setFloat("material.roughnessScalar", roughnessScalar);
    

    if (usingAmbientOcclusionMap && ambientOcclusionMap)
    {
        ambientOcclusionMap->bind(4);
        shader->setInt("material.ao", 4);
        shader->setInt("material.usingAo", 1);
    }
    else
    {
        shader->setInt("material.usingAo", 0);
    }
    

    if (usingDepthMap && depthMap)
    {
        depthMap->bind(5);
        shader->setInt("material.depth", 5);
        shader->setInt("material.usingDepth", 1);
    }
    else
    {
        shader->setInt("material.usingDepth", 0);
    }
}

void Material::unbind() const
{
    if (shader)
    {
        shader->unbind();
    }

    albedoMap->unbind(0);
    normalMap->unbind(1);
    metallicMap->unbind(2);
    roughnessMap->unbind(3);
    ambientOcclusionMap->unbind(4);
    depthMap->unbind(5);
}

Shared<Material> Material::create(const Shared<Shader>& shader)
{
    auto material = createShared<Material>();

    if (shader)
    {
        material->shader = shader;
    }

    /*uint32_t white = 0xffffffff; // TODO: don't bother setting texture colors, leave them as nullptr's
    uint32_t black = 0x00000000;
    
    material->albedoMap = Texture2D::create(1, 1);
    material->albedoMap->setData(0, 0, 1, 1, &white);

    material->normalMap = Texture2D::create(1, 1);
    material->normalMap->setData(0, 0, 1, 1, &black);

    material->metallicMap = Texture2D::create(1, 1);
    material->metallicMap->setData(0, 0, 1, 1, &white);

    material->roughnessMap = Texture2D::create(1, 1);
    material->roughnessMap->setData(0, 0, 1, 1, &white);

    material->ambientOcclusionMap = Texture2D::create(1, 1);
    material->ambientOcclusionMap->setData(0, 0, 1, 1, &white);

    material->depthMap = Texture2D::create(1, 1);
    material->depthMap->setData(0, 0, 1, 1, &white);

    material->albedoColor = math::vec4(1, 1, 1, 1);*/

    return material;
}

Shared<Material> Material::create()
{
    return Material::create(nullptr);
}

}