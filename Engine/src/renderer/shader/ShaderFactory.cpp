#include <renderer/shader/ShaderFactory.h>

Shared<Shader> ShaderFactory::textureShader()
{
    return Shader::createFromFile("Engine/shaders/texture.glsl");
}

Shared<Shader> ShaderFactory::textShader()
{
    return Shader::createFromFile("Engine/shaders/text.glsl");
}

Shared<Shader> ShaderFactory::framebufferShader()
{
    return Shader::createFromFile("Engine/shaders/framebuffer.glsl");
}

Shared<Shader> ShaderFactory::lightingShader()
{
    return Shader::createFromFile("Engine/shaders/lighting.glsl");
}