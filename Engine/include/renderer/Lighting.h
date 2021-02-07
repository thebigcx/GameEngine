#pragma once

#include <vector>
#include <iostream>

#include <maths/math.h>

#include <core/Logger.h>
#include <renderer/shader/Shader.h>
#include <scene/GameComponent.h>

namespace Engine
{

struct ShadowInfo
{
public:
    ShadowInfo(const math::mat4& projection)
        : m_projection(projection) {}

    inline const math::mat4& getProjection() { return m_projection; }

private:
    math::mat4 m_projection;
};

struct BaseLight
{
public:
    BaseLight() {}
    BaseLight(const math::vec3& radiance_, float intensity_)
        : radiance(radiance_), intensity(intensity_), m_shadowInfo(nullptr) {}

    virtual void setShaderUniforms(const Reference<Shader>& shader, uint32_t index) const {};

    math::vec3 radiance = math::vec3(1, 1, 1);
    float intensity = 1.f;

    virtual ~BaseLight() = default;

    inline ShadowInfo* getShadowInfo() { return m_shadowInfo; }
    void setShadowInfo(ShadowInfo* info);

private:
    ShadowInfo* m_shadowInfo;
};

struct PointLight : public BaseLight, public GameComponent
{
public:
    PointLight() {}
    PointLight(const math::vec3& radiance, float intensity, const math::vec3& position_)
        : BaseLight(radiance, intensity) {}

    void setShaderUniforms(const Reference<Shader>& shader, uint32_t index) const override;

    //math::vec3 position;
};
// TODO: possibly add support for multiple directional lights
struct DirectionalLight : public BaseLight, public GameComponent
{
public:
    DirectionalLight() {}
    DirectionalLight(const math::vec3& radiance, float intensity, const math::vec3& direction_);

    void setShaderUniforms(const Reference<Shader>& shader, uint32_t index) const override;

    math::vec3 direction;
};
// TODO: make spotlight compatible with pbr shading
struct SpotLight : public BaseLight, public GameComponent
{
public:
    SpotLight() {}
    SpotLight(const math::vec3& radiance, float intensity, const math::vec3& position_, const math::vec3& direction_,
              float cutoff_, float outerCutoff_)
        : BaseLight(radiance, intensity), position(position_), cutoff(cutoff_), outerCutoff(outerCutoff_) {}

    math::vec3 position;
    //math::vec3 direction;
    float cutoff;
    float outerCutoff;
};

struct SkyLight : public BaseLight, public GameComponent
{
public:
    SkyLight() {}
    SkyLight(const math::vec3& radiance, float intensity)
        : BaseLight(radiance, intensity) {}

    void setShaderUniforms(const Reference<Shader>& shader, uint32_t index) const override;
};

}