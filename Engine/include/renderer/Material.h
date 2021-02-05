#pragma once

#include <vector>

#include <core/Core.h>
#include <renderer/shader/Shader.h>
#include <renderer/Texture2D.h>

namespace Engine
{

class Material
{
public:
    void bind() const;
    void unbind() const;

    static Reference<Material> createFromFile(const std::string& path);

    static Reference<Material> create();
    static Reference<Material> create(const Reference<Shader>& shader);

    static Reference<Material> create(const Reference<Shader>& shader,
                                   const Reference<Texture2D>& albedo,
                                   const Reference<Texture2D>& normal,
                                   const Reference<Texture2D>& metallic,
                                   const Reference<Texture2D>& roughness,
                                   const Reference<Texture2D>& ao,
                                   const Reference<Texture2D>& depth,
                                   const Reference<Texture2D>& emission);

    float metallicScalar = 0.f;
    float roughnessScalar = 0.f;
    Reference<Texture2D> albedoMap = nullptr;
    Reference<Texture2D> normalMap = nullptr;
    Reference<Texture2D> metallicMap = nullptr;
    Reference<Texture2D> roughnessMap = nullptr;
    Reference<Texture2D> ambientOcclusionMap = nullptr;
    Reference<Texture2D> depthMap = nullptr;
    Reference<Texture2D> emissionMap = nullptr;

    math::vec4 albedoColor = math::vec4(1.f);

    Reference<Shader> shader = nullptr;

    std::string name = "";
    std::string uuid = "";

    bool operator==(const Material& other)
    {
        return other.albedoMap == albedoMap && 
               other.normalMap == normalMap && 
               other.metallicMap == metallicMap && 
               other.roughnessMap == roughnessMap &&
               other.ambientOcclusionMap == ambientOcclusionMap &&
               other.depthMap == ambientOcclusionMap &&
               other.emissionMap == emissionMap &&
               other.metallicScalar == metallicScalar &&
               other.roughnessScalar == roughnessScalar;
    }

    bool operator!=(const Material& other)
    {
        return !(*this == other);
    }
};

}