#pragma once

#include <vector>
#include <iostream>

#include <maths/vector/vec3.h>

struct PointLight
{
    //math::vec3 position, ambient, diffuse, specular;
    //float constant, linear, quadratic;
    math::vec3 position, color;
    float intensity;
};

struct DirectionalLight
{
    //math::vec3 direction, ambient, diffuse, specular;
    math::vec3 direction;
    math::vec3 color;
    float intensity;
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

    float getSkyLight() const
    {
        return m_skyLight;
    }

private:
    DirectionalLight m_dirLight;
    std::vector<PointLight> m_pointLights;
    float m_skyLight = 0;
};