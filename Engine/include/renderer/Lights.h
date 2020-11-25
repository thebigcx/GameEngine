#pragma once

#include <vector>
#include <iostream>

#include <maths/maths/vector/vec3.h>

struct PointLight
{
    math::vec3 position, ambient, diffuse, specular;
    float constant, linear, quadratic;
};

struct DirectionalLight
{
    math::vec3 direction, ambient, diffuse, specular;
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

private:
    DirectionalLight m_dirLight;
    std::vector<PointLight> m_pointLights;
};