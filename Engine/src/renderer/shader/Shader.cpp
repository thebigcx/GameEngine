#include <vector>

#include <renderer/shader/Shader.h>
#include <util/Timer.h>
#include <platform/GLShader.h>

Shared<Shader> Shader::createFromFile(const std::string& vsPath, const std::string& fsPath)
{
    return createShared<GLShader>(vsPath, fsPath);
}