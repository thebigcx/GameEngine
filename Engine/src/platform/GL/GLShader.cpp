#include <platform/GL/GLShader.h>
#include <util/io/Files.h>

#include <cstring>

GLShader::GLShader(const std::string& path)
{
    std::string source = Files::readFile(path);
    ShaderSource shaderSource = preProcess(source);
    compileShader(shaderSource);
}

GLShader::GLShader(const std::string& vertSource, const std::string& fragSource)
{
    ShaderSource source = { vertSource, fragSource };
    compileShader(source);
}

GLShader::~GLShader()
{
    if (m_id != 0)
    {
        glDeleteProgram(m_id);
    }
}

ShaderSource GLShader::preProcess(const std::string& source)
{
    ShaderSource shaderSource;

    const char* typeFlag = "#shader";

    size_t pos = source.find(typeFlag);

    while (pos != std::string::npos)
    {
        size_t begin = pos + std::strlen(typeFlag) + 1;
        size_t eol = source.find_first_of("\n", pos);
        std::string type = source.substr(begin, eol - begin);

        size_t nextLine = source.find_first_not_of("\n", eol);
        pos = source.find(typeFlag, nextLine);

        if (pos == std::string::npos)
        {
            if (type == "vertex")
                shaderSource.vertex = source.substr(nextLine);
            else if (type == "fragment")
                shaderSource.fragment = source.substr(nextLine);
        }
        else
        {
            if (type == "vertex")
                shaderSource.vertex = source.substr(nextLine, pos - nextLine);
            else if (type == "fragment")
                shaderSource.fragment = source.substr(nextLine, pos - nextLine);
        }
    }

    return shaderSource;
}

bool GLShader::compileShader(const ShaderSource& source)
{
    const char* vShaderCode = source.vertex.c_str();
    const char* fShaderCode = source.fragment.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);

    // Check compile errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "Failed to compile vertex shader.\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);

    // Check compile errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "Failed to compile fragment shader.\n" << infoLog << std::endl;
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);

    // Check link errors
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
        std::cout << "Failed to link shader program.\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return success;
}

void GLShader::bind() const
{
    if (m_id != 0)
    {
        glUseProgram(m_id);
    }
}

void GLShader::unbind() const
{
    glUseProgram(0);
}

void GLShader::setInt(const std::string& name, int value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform1i(location, value);
}
    
void GLShader::setInt2(const std::string& name, const math::ivec2& value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform2i(location, value.x, value.y);
}

void GLShader::setInt3(const std::string& name, const math::ivec3& value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform3i(location, value.x, value.y, value.z);
}

void GLShader::setInt4(const std::string& name, const math::ivec4& value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform4i(location, value.x, value.y, value.z, value.w);
}

void GLShader::setFloat(const std::string& name, float value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform1f(location, value);
}

void GLShader::setFloat2(const std::string& name, const math::vec2& value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform2f(location, value.x, value.y);
}

void GLShader::setFloat3(const std::string& name, const math::vec3& value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void GLShader::setFloat4(const std::string& name, const math::vec4& value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void GLShader::setUint(const std::string& name, unsigned int value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform1ui(location, value);
}

void GLShader::setUint2(const std::string& name, math::uvec2& value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform2ui(location, value.x, value.y);
}

void GLShader::setUint3(const std::string& name, math::uvec3& value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform3ui(location, value.x, value.y, value.z);
}

void GLShader::setUint4(const std::string& name, math::uvec4& value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform4ui(location, value.x, value.y, value.z, value.w);
}

void GLShader::setFloatArray(const std::string& name, float* value, uint32_t count)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform1fv(location, count, value);
}

void GLShader::setFloat2Array(const std::string& name, math::vec2* value, uint32_t count)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform2fv(location, count, &(value->x));
}

void GLShader::setFloat3Array(const std::string& name, math::vec3* value, uint32_t count)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform3fv(location, count, &(value->x));
}

void GLShader::setFloat4Array(const std::string& name, math::vec4* value, uint32_t count)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform4fv(location, count, &(value->x));
}

void GLShader::setIntArray(const std::string& name, int* value, uint32_t count)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform1iv(location, count, value);
}

void GLShader::setInt2Array(const std::string& name, math::ivec2* value, uint32_t count)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform2iv(location, count, &(value->x));
}

void GLShader::setInt3Array(const std::string& name, math::ivec3* value, uint32_t count)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform3iv(location, count, &(value->x));
}

void GLShader::setInt4Array(const std::string& name, math::ivec4* value, uint32_t count)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform4iv(location, count, &(value->x));
}

void GLShader::setUintArray(const std::string& name, unsigned int* value, uint32_t count)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform1uiv(location, count, value);
}

void GLShader::setMatrix3(const std::string& name, const math::mat3& value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, math::buffer(value));
}

void GLShader::setMatrix4(const std::string& name, const math::mat4& value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, math::buffer(value));
}