#pragma once

#include <renderer/shader/Shader.h>
#include <core/Core.h>

namespace Engine
{

class ShaderFactory
{
public:
    static Shared<Shader> textureShader();
    static Shared<Shader> textShader();
    static Shared<Shader> framebufferShader();
    static Shared<Shader> lightingShader();
    static Shared<Shader> createShader(const std::string& name);
};

}