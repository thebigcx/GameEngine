#pragma once

#include <vector>
#include <iostream>

#include <maths/vector/vec3.h>

#include <core/Logger.h>

struct PointLight
{
    math::vec3 position, color;
    float intensity;
    float specular;
    float attenuation;
};

struct DirectionalLight
{
    alignas(16) math::vec3 direction;
    alignas(16) math::vec3 color;
    alignas(4) float intensity;
    alignas(4) float specular;
};

struct SpotLight
{
    math::vec3 position, direction, color;
    float cutoff;
    float outerCutoff;
    float intensity;
    float specular;
    float attenuation;
};

class LightSetup
{
public:
    LightSetup() {}

    void setDirectionalLight(const DirectionalLight& light)
    {
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

    float getSkyLight() const
    {
        return m_skyLight;
    }

private:
    DirectionalLight m_dirLight;
    std::vector<PointLight> m_pointLights;
    std::vector<SpotLight> m_spotLights;
    float m_skyLight = 0;
};

struct alignas(16) LightStruct
{
    alignas(64) DirectionalLight dirLight;
    //PointLight pointLights[64];
    //SpotLight spotLights[64];

    //int numPointLights;
    //int numSpotLights;

    alignas(16) math::vec3 cameraPos;
    alignas(4) float skyLight;
};