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

    static Shared<Material> create();
    static Shared<Material> create(const Shared<Shader>& shader);

    static Shared<Material> create(const Shared<Shader>& shader,
                                   const Shared<Texture2D>& albedo,
                                   const Shared<Texture2D>& normal,
                                   const Shared<Texture2D>& metallic,
                                   const Shared<Texture2D>& roughness,
                                   const Shared<Texture2D>& ao,
                                   const Shared<Texture2D>& depth);

    bool usingAlbedoMap = true;
    bool usingNormalMap = false;
    bool usingMetallicMap = false;
    bool usingRoughnessMap = false;
    bool usingAmbientOcclusionMap = false;
    bool usingDepthMap = false;

    float metallicScalar = 0.f;
    float roughnessScalar = 0.f;
    Shared<Texture2D> albedoMap = nullptr;
    Shared<Texture2D> normalMap = nullptr;
    Shared<Texture2D> metallicMap = nullptr;
    Shared<Texture2D> roughnessMap = nullptr;
    Shared<Texture2D> ambientOcclusionMap = nullptr;
    Shared<Texture2D> depthMap = nullptr;

    math::vec4 albedoColor = math::vec4(1.f);

    Shared<Shader> shader;

    bool operator==(const Material& other)
    {
        return other.albedoMap == albedoMap && 
               other.normalMap == normalMap && 
               other.metallicMap == metallicMap && 
               other.roughnessMap == roughnessMap &&
               other.ambientOcclusionMap == ambientOcclusionMap &&
               other.depthMap == ambientOcclusionMap &&

               other.usingAlbedoMap == usingAlbedoMap &&
               other.usingNormalMap == usingNormalMap &&
               other.usingMetallicMap == usingMetallicMap &&
               other.usingRoughnessMap == usingRoughnessMap &&
               other.usingAmbientOcclusionMap == usingAmbientOcclusionMap &&
               other.usingDepthMap == usingDepthMap &&

               other.metallicScalar == metallicScalar &&
               other.roughnessScalar == roughnessScalar;
    }

    bool operator!=(const Material& other)
    {
        return !(*this == other);
    }
};

}