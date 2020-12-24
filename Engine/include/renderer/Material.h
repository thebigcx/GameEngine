#pragma once

#include <vector>

#include <core/Core.h>
#include <renderer/shader/Shader.h>
#include <renderer/Texture2D.h>

class Material
{
public:
    void bind() const;
    void unbind() const;

    static Shared<Material> create();
    static Shared<Material> create(const Shared<Shader>& shader);

    bool usingMetallicMap = false;
    bool usingRoughnessMap = false;
    bool usingAlbedoMap = true;
    bool usingNormalMap = false;
    bool usingAmbientOcclusionMap = false;
    bool usingDepthMap = false;

    float metalness = 0.f;
    float roughness = 0.f;
    Shared<Texture2D> albedoMap;
    Shared<Texture2D> normalMap;
    Shared<Texture2D> metallicMap;
    Shared<Texture2D> roughnessMap;
    Shared<Texture2D> ambientOcclusionMap;
    Shared<Texture2D> depthMap;

    math::vec4 albedoColor;

    Shared<Shader> shader;

    bool operator==(const Material& other)
    {
        return other.albedoMap == albedoMap && 
               other.normalMap == normalMap && 
               other.metallicMap == metallicMap && 
               other.roughnessMap == roughnessMap &&
               other.ambientOcclusionMap == ambientOcclusionMap &&
               other.depthMap == ambientOcclusionMap;
    }

    bool operator!=(const Material& other)
    {
        return other.albedoMap != albedoMap || 
               other.normalMap != normalMap || 
               other.metallicMap != metallicMap || 
               other.roughnessMap != roughnessMap ||
               other.ambientOcclusionMap != ambientOcclusionMap ||
               other.depthMap != ambientOcclusionMap;
    }
};