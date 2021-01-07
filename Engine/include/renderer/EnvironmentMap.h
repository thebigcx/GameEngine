#pragma once

#include <core/Core.h>
#include <renderer/TextureCube.h>
#include <renderer/shader/Shader.h>
#include <renderer/Mesh.h>

namespace Engine
{

class EnvironmentMap
{
public:
    EnvironmentMap(const std::string& hdrFile);

    inline constexpr const Shared<TextureCube>& getEnvMap() const noexcept { return m_envMap; }
    inline constexpr const Shared<TextureCube>& getIrradiance() const noexcept { return m_irradianceMap; }
    inline constexpr const Shared<TextureCube>& getPrefilter() const noexcept { return m_prefilterMap; }
    inline constexpr const Shared<Texture2D>& getBRDF() const noexcept { return m_brdfLUTTexture; }

    static Shared<EnvironmentMap> create(const std::string& file);

private:
    Shared<TextureCube> m_envMap;
    Shared<TextureCube> m_irradianceMap;
    Shared<TextureCube> m_prefilterMap;

    Shared<Texture2D> m_brdfLUTTexture;

    static inline Shared<Shader> s_convertShader = nullptr;
    static inline Shared<Shader> s_irradianceShader = nullptr;
    static inline Shared<Shader> s_prefilterShader = nullptr;
    static inline Shared<Shader> s_brdfShader = nullptr;

    static inline Shared<Mesh> s_cubeMesh = nullptr;
};

}