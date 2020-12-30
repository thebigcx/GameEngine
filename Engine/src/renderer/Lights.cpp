#include <renderer/Lights.h>
#include <renderer/Renderer3D.h>

namespace Engine
{

void BaseLight::setShadowInfo(ShadowInfo* info)
{
    if (m_shadowInfo) delete m_shadowInfo;
    m_shadowInfo = info;
}

void BaseLight::addToRenderer()
{
    Renderer3D::addLight(this);
}

DirectionalLight::DirectionalLight(const math::vec3& radiance, float intensity, const math::vec3& direction_)
    : BaseLight(radiance, intensity), direction(direction_)
{
    setShadowInfo(new ShadowInfo(math::ortho(-40.f, 40.f, -40.f, 40.f, -40.f, 40.f)));
}

void DirectionalLight::setShaderUniforms(const Shared<Shader>& shader, uint32_t index) const
{
    shader->setFloat3("directionalLight.radiance", radiance);
    shader->setFloat("directionalLight.intensity", intensity);
    shader->setFloat3("directionalLight.direction", direction);
}

void PointLight::setShaderUniforms(const Shared<Shader>& shader, uint32_t index) const
{
    std::string idx = std::to_string(index);

    shader->setFloat3("pointLights[" + idx + "].radiance", radiance);
    shader->setFloat("pointLights[" + idx + "].intensity", intensity);
    shader->setFloat3("pointLights[" + idx + "].position", position);
    shader->setFloat("pointLights[" + idx + "].attenuation", attenuation);
}

void SkyLight::setShaderUniforms(const Shared<Shader>& shader, uint32_t index) const
{
    shader->setFloat3("skyLight.radiance", radiance);
    shader->setFloat("skyLight.intensity", intensity);
}

}