#include <renderer/shader/ShaderFactory.h>

Shared<Shader> ShaderFactory::textureShader()
{
    auto shader = Shader::createFromFile("shaders/texture.glsl");
    return shader;
}

Shared<Shader> ShaderFactory::textShader()
{
    auto shader = Shader::createFromFile("shaders/text.glsl");
    return shader;
}

Shared<Shader> ShaderFactory::framebufferShader()
{
    auto shader = Shader::createFromFile("shaders/framebuffer.glsl");
    return shader;
}