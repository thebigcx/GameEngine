#pragma once

#include <vector>
#include <iostream>

#include <maths/vector/vec3.h>

#include <core/Logger.h>

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

    virtual void addToShader(const Shared<Shader>& shader, uint32_t index) {}

    math::vec3 radiance;
    float intensity;

    virtual ~BaseLight() = default;

private:
    ShadowInfo* m_shadowInfo;
};

struct PointLight : public BaseLight
{
public:
    PointLight() {}
    PointLight(const math::vec3& radiance, float intensity, const math::vec3& position_, float attenuation_)
        : BaseLight(radiance, intensity), position(position_), attenuation(attenuation_) {}

    void addToShader(const Shared<Shader>& shader, uint32_t index) override
    {
        std::string idx = std::to_string(index);

        shader->setFloat3("pointLights[" + idx + "].radiance", radiance);
        shader->setFloat("pointLights[" + idx + "].intensity", intensity);
        shader->setFloat3("pointLights[" + idx + "].position", position);
        shader->setFloat("pointLights[" + idx + "].attenuation", attenuation);
    }

    math::vec3 position;
    float attenuation;
};
// TODO: possibly add support for multiple directional lights
struct DirectionalLight : public BaseLight
{
public:
    DirectionalLight() {}
    DirectionalLight(const math::vec3& radiance, float intensity, const math::vec3& direction_)
        : BaseLight(radiance, intensity), direction(direction_) {}

    void addToShader(const Shared<Shader>& shader, uint32_t index) override
    {
        shader->setFloat3("directionalLight.radiance", radiance);
        shader->setFloat("directionalLight.intensity", intensity);
        shader->setFloat3("directionalLight.direction", direction);
    }

    math::vec3 direction;
};
// TODO: make spotlight compatible with pbr shading
struct SpotLight : public BaseLight
{
public:
    SpotLight() {}
    SpotLight(const math::vec3& radiance, float intensity, const math::vec3& position_, const math::vec3& direction_,
              float cutoff_, float outerCutoff_)
        : BaseLight(radiance, intensity), position(position_), direction(direction_), cutoff(cutoff_), outerCutoff(outerCutoff_) {}
    // TODO: addToShader method
    math::vec3 position;
    math::vec3 direction;
    float cutoff;
    float outerCutoff;
};

struct SkyLight : public BaseLight
{
public:
    SkyLight() {}
    SkyLight(const math::vec3& radiance, float intensity)
        : BaseLight(radiance, intensity) {}

    void addToShader(const Shared<Shader>& shader, uint32_t index) override
    {
        shader->setFloat3("skyLight.radiance", radiance);
        shader->setFloat("skyLight.intensity", intensity);
    }
};
/*
class LightSetup
{
public:
    LightSetup() {}

    bool hasLights() const
    {
        return m_usingDirectionalLight || m_pointLights.size() > 0 || m_spotLights.size() > 0 || m_skyLight > 0.f;
    }

    void setDirectionalLight(const DirectionalLight& light)
    {
        m_usingDirectionalLight = true;
        m_dirLight = light;
    }

    void setPointLights(const std::vector<PointLight>& lights)
    {
        m_pointLights.insert(m_pointLights.begin(), lights.begin(), lights.end());
        if (m_pointLights.size() > 64)
        {
            Logger::getCoreLogger()->error("Too many point lights: %i", m_pointLights.size());
        }
    }

    void setSpotLights(const std::vector<SpotLight>& lights)
    {
        m_spotLights.insert(m_spotLights.begin(), lights.begin(), lights.end());
        if (m_spotLights.size() > 64)
        {
            Logger::getCoreLogger()->error("Too many spot lights: %i", m_spotLights.size());
        }
    }

    void setSkylight(float intensity)
    {
        m_skyLight = intensity;
    }

    void removePointLight(int index)
    {
        m_pointLights.erase(m_pointLights.begin() + index);
    }

    const DirectionalLight& getDirectionalLight() const
    {
        return m_dirLight;
    }

    const std::vector<PointLight>& getPointLights() const
    {
        return m_pointLights;
    }

    const std::vector<SpotLight>& getSpotLights() const
    {
        return m_spotLights;
    }

    const bool& usingDirectionalLight() const
    {
        return m_usingDirectionalLight;
    }

    float getSkyLight() const
    {
        return m_skyLight;
    }

private:
    DirectionalLight m_dirLight;
    std::vector<PointLight> m_pointLights;
    std::vector<SpotLight> m_spotLights;
    float m_skyLight = 0.f;
    bool m_usingDirectionalLight = false;
};
*/
}