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

    inline constexpr const Reference<TextureCube>& getEnvMap() const noexcept { return m_envMap; }
    inline constexpr const Reference<TextureCube>& getIrradiance() const noexcept { return m_irradianceMap; }
    inline constexpr const Reference<TextureCube>& getPrefilter() const noexcept { return m_prefilterMap; }
    inline constexpr const Reference<Texture2D>& getBRDF() const noexcept { return m_brdfLUT; }

    static Reference<EnvironmentMap> create(const std::string& file);

private:
    Reference<TextureCube> m_envMap;
    Reference<TextureCube> m_irradianceMap;
    Reference<TextureCube> m_prefilterMap;

    Reference<Texture2D> m_brdfLUT;

    static inline Reference<Shader> s_convertShader = nullptr;
    static inline Reference<Shader> s_irradianceShader = nullptr;
    static inline Reference<Shader> s_prefilterShader = nullptr;
    static inline Reference<Shader> s_brdfShader = nullptr;

    static inline Reference<Mesh> s_cubeMesh = nullptr;

    static inline math::mat4 s_captureProjection;
    static inline std::array<math::mat4, 6> s_captureViews;

    static void initialize();

    static Reference<TextureCube> hdrToCubemap(const std::string& hdrFile);
    static Reference<TextureCube> createIrradianceMap(const Reference<TextureCube>& envMap);
    static Reference<TextureCube> createPrefilterMap(const Reference<TextureCube>& envMap, const Reference<TextureCube>& irradianceMap);
    static Reference<Texture2D> createBRDFLUT();
};

}