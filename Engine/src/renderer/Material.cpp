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

    int32_t textureFlags = 0;

    if (albedoMap)
    {
        albedoMap->bind(0);
        textureFlags |= (1 << 0);
    }
    else
    {
        shader->setFloat3("material.albedoColor", albedoColor);
    }

    if (normalMap)
    {
        normalMap->bind(1);
        textureFlags |= (1 << 1);
    }

    if (metallicMap)
    {
        metallicMap->bind(2);
        textureFlags |= (1 << 2);
    }
    shader->setFloat("material.metallicScalar", metallicScalar);
    

    if (roughnessMap)
    {
        roughnessMap->bind(3);
        textureFlags |= (1 << 3);
    }
    shader->setFloat("material.roughnessScalar", roughnessScalar);
    

    if (ambientOcclusionMap)
    {
        ambientOcclusionMap->bind(4);
        textureFlags |= (1 << 4);
    }
    

    if (depthMap)
    {
        depthMap->bind(5);
        textureFlags |= (1 << 5);
    }

    if (emissionMap)
    {
        emissionMap->bind(6);
        textureFlags |= (1 << 6);
    }

    shader->setInt("material.textureFlags", textureFlags);
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
    emissionMap->unbind(6);
}

Shared<Material> Material::create(const Shared<Shader>& shader)
{
    auto material = createShared<Material>();

    if (shader)
    {
        material->shader = shader;
    }

    return material;
}

Shared<Material> create(const Shared<Shader>& shader,
                        const Shared<Texture2D>& albedo,
                        const Shared<Texture2D>& normal,
                        const Shared<Texture2D>& metallic,
                        const Shared<Texture2D>& roughness,
                        const Shared<Texture2D>& ao,
                        const Shared<Texture2D>& depth,
                        const Shared<Texture2D>& emission)
{
    auto material = createShared<Material>();
    
    material->shader = shader;
    material->albedoMap = albedo;
    material->normalMap = normal;
    material->metallicMap = metallic;
    material->roughnessMap = roughness;
    material->ambientOcclusionMap = ao;
    material->depthMap = depth;
    material->emissionMap = emission;

    return material;
}

Shared<Material> Material::create()
{
    return Material::create(nullptr);
}

}