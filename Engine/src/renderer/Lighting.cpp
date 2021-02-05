#include <renderer/Lighting.h>
#include <renderer/Renderer3D.h>
#include <util/Transform.h>

namespace Engine
{

void BaseLight::setShadowInfo(ShadowInfo* info)
{
    if (m_shadowInfo) delete m_shadowInfo;
    m_shadowInfo = info;
}

DirectionalLight::DirectionalLight(const math::vec3& radiance, float intensity, const math::vec3& direction_)
    : BaseLight(radiance, intensity), direction(direction_)
{
    setShadowInfo(new ShadowInfo(math::ortho(-40.f, 40.f, -40.f, 40.f, -40.f, 40.f)));
}

void DirectionalLight::setShaderUniforms(const Reference<Shader>& shader, uint32_t index) const
{
    shader->setFloat3("uDirectionalLight.radiance", radiance);
    shader->setFloat("uDirectionalLight.intensity", intensity);
    shader->setFloat3("uDirectionalLight.direction", direction);
}

void DirectionalLight::onTransformChange(const Transform& transform)
{
    math::quat rotation = math::quat(math::radians(transform.getRotation()));
    direction = rotation * math::vec3(0, -1, 0);
}

void PointLight::setShaderUniforms(const Reference<Shader>& shader, uint32_t index) const
{
    std::string idx = std::to_string(index);

    shader->setFloat3("uPointLights[" + idx + "].radiance", radiance);
    shader->setFloat("uPointLights[" + idx + "].intensity", intensity);
    shader->setFloat3("uPointLights[" + idx + "].position", position);
}

void PointLight::onTransformChange(const Transform& transform)
{
    position = transform.getTranslation();
    std::cout << math::to_string(position) << "\n";
}

void SkyLight::setShaderUniforms(const Reference<Shader>& shader, uint32_t index) const
{
    shader->setFloat3("uSkyLight.radiance", radiance);
    shader->setFloat("uSkyLight.intensity", intensity);
}

void SpotLight::onTransformChange(const Transform& transform)
{

}

}