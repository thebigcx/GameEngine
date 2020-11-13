#include <vector>

#include <renderer/shader/Shader.h>
#include <util/Timer.h>
#include <platform/GLShader.h>

Shared<Shader> Shader::createFromFile(const std::string& path)
{
    return createShared<GLShader>(path);
}

Shared<Shader> Shader::createFromSource(const std::string& vertSource, const std::string& fragSource)
{
    return createShared<GLShader>(vertSource, fragSource);
}