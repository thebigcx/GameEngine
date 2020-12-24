#include <renderer/shader/ShaderFactory.h>

namespace Engine
{

Shared<Shader> ShaderFactory::textureShader()
{
    return createShader("texture");;
}

Shared<Shader> ShaderFactory::textShader()
{
    return createShader("text");
}

Shared<Shader> ShaderFactory::framebufferShader()
{
    return createShader("framebuffer");
}

Shared<Shader> ShaderFactory::lightingShader()
{
    return createShader("lighting");
}

Shared<Shader> ShaderFactory::createShader(const std::string& name)
{
    return Shader::createFromFile("Engine/src/renderer/shader/default/" + name + ".glsl");
}

}