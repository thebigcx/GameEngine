#pragma once

#include <renderer/shader/Shader.h>
#include <core/Core.h>

namespace Engine
{

class ShaderFactory
{
public:
    static Reference<Shader> textureShader();
    static Reference<Shader> textShader();
    static Reference<Shader> framebufferShader();
    static Reference<Shader> lightingShader();
    static Reference<Shader> createShader(const std::string& name);
};

}