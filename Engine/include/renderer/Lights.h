#pragma once

#include <vector>
#include <iostream>

#include <maths/vector/vec3.h>

struct PointLight
{
    math::vec3 position, color;
    float intensity;
    float specular;
    float attenuation;
};

struct DirectionalLight
{
    math::vec3 direction;
    math::vec3 color;
    float intensity;
    float specular;
};

struct SpotLight
{
    math::vec3 position, direction;
    math::vec3 color;
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
            std::cout << "Too many point lights!\n\n";
        }
    }

    void setSpotLights(const std::vector<SpotLight>& lights)
    {
        m_spotLights.insert(m_spotLights.begin(), lights.begin(), lights.end());
        if (m_spotLights.size() > 64)
        {
            std::cout << "Too many spot lights!\n\n";
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