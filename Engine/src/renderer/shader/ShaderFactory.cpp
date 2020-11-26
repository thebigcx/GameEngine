#include <renderer/shader/ShaderFactory.h>

Shared<Shader> ShaderFactory::textureShader()
{
    return Shader::createFromFile("Engine/src/renderer/shader/default/texture.glsl");
}

Shared<Shader> ShaderFactory::textShader()
{
    return Shader::createFromFile("Engine/src/renderer/shader/default/text.glsl");
}

Shared<Shader> ShaderFactory::framebufferShader()
{
    return Shader::createFromFile("Engine/src/renderer/shader/default/framebuffer.glsl");
}

Shared<Shader> ShaderFactory::lightingShader()
{
    return Shader::createFromFile("Engine/src/renderer/shader/default/lighting.glsl");
}