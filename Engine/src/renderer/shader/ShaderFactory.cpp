#include <renderer/shader/ShaderFactory.h>

Shared<Shader> ShaderFactory::textureShader()
{
    auto shader = Shader::createFromFile("shaders/texture.vert", "shaders/texture.frag");
    return shader;
}

Shared<Shader> ShaderFactory::textShader()
{
    auto shader = Shader::createFromFile("shaders/text.vert", "shaders/text.frag");
    return shader;
}