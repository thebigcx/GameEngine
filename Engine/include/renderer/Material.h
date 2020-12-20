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

    bool usingMetalnessMap = false;
    bool usingRoughnessMap = false;
    bool usingAlbedoMap = true;
    bool usingNormalMap = false;
    bool usingAmbientOcclusionMap = false;
    bool usingDepthMap = false;

    float metalness = 0.f;
    float roughness = 0.f;
    Shared<Texture2D> albedoMap;
    Shared<Texture2D> normalMap;
    Shared<Texture2D> metalnessMap;
    Shared<Texture2D> roughnessMap;
    Shared<Texture2D> ambientOcclusionMap;
    Shared<Texture2D> depthMap;

    math::vec4 albedoColor;

    Shared<Shader> shader;
};