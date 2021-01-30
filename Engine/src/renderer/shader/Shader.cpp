#include <renderer/shader/Shader.h>
#include <platform/GL/GLShader.h>

namespace Engine
{

Reference<Shader> Shader::createFromFile(const std::string& path)
{
    return createReference<GLShader>(path);
}

Reference<Shader> Shader::createFromSource(const std::string& vertSource, const std::string& fragSource)
{
    return createReference<GLShader>(vertSource, fragSource);
}

Reference<Shader> Shader::createFromFileWithMacros(const std::string& path, const std::unordered_map<std::string, std::string>& macros)
{
    return createReference<GLShader>(path, macros);
}

}