#pragma once

#include <vector>
#include <iostream>

#include <maths/vector/vec3.h>

#include <core/Logger.h>

namespace Engine
{

struct PointLight
{
    math::vec3 position;
    math::vec3 radiance;
    float intensity;
    float attenuation;
};

struct DirectionalLight
{
    math::vec3 direction;
    math::vec3 radiance;
    float intensity;
};

struct SpotLight
{
    math::vec3 position, direction, radiance;
    float cutoff;
    float outerCutoff;
    float intensity;
};

class LightSetup
{
public:
    LightSetup() {}

    bool hasLights() const
    {
        return m_usingDirectionalLight || m_pointLights.size() > 0 || m_spotLights.size() > 0 || m_skyLight > 0;
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
    float m_skyLight = 0;
    bool m_usingDirectionalLight = false;
};

}