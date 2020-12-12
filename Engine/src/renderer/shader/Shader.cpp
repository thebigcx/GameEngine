#include <vector>

#include <renderer/shader/Shader.h>
#include <util/Timer.h>
#include <platform/GL/GLShader.h>

Shared<Shader> Shader::createFromFile(const std::string& path)
{
    return createShared<GLShader>(path);
}

Shared<Shader> Shader::createFromSource(const std::string& vertSource, const std::string& fragSource)
{
    return createShared<GLShader>(vertSource, fragSource);
}

Shared<Shader> Shader::createFromFileWithMacros(const std::string& path, const std::unordered_map<std::string, std::string>& macros)
{
    return createShared<GLShader>(path, macros);
}