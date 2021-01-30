#include <renderer/shader/ShaderFactory.h>

namespace Engine
{

Reference<Shader> ShaderFactory::textureShader()
{
    return createShader("texture");
}

Reference<Shader> ShaderFactory::textShader()
{
    return createShader("text");
}

Reference<Shader> ShaderFactory::framebufferShader()
{
    return createShader("framebuffer");
}

Reference<Shader> ShaderFactory::lightingShader()
{
    return createShader("lighting");
}

Reference<Shader> ShaderFactory::createShader(const std::string& name)
{
    return Shader::createFromFile("Engine/assets/shaders/" + name + ".glsl");
}

}