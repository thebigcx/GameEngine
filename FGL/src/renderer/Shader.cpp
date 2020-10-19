#include "renderer/Shader.h"

#include "util/Timer.h"

Shader::Shader()
{

}

Shader::Shader(const std::string& vsPath, const std::string& fsPath)
{
    create(vsPath, fsPath);
}

void Shader::create(const std::string& vsPath, const std::string& fsPath)
{
    auto source = parseShader(vsPath, fsPath);
    bool success = compileShader(source);

    // Find all uniforms
    int i;
    int count;
    int size;
    GLenum type;
    const GLsizei bufSize = 16;
    GLchar name[bufSize];
    GLsizei length;

    glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTES, &count);

    for (i = 0 ; i < count ; i++)
    {
        glGetActiveUniform(m_id, (GLuint)i, bufSize, &length, &size, &type, name);

        Uniform uniform;
        uniform = { name, (size_t)size, type, i };

        m_uniforms.insert(std::make_pair(std::string(name), uniform));
    }
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}

Shader::Shader(const Shader& shader)
{
    m_id = shader.getId();
}

Shader::Shader(Shader&& shader)
{
    m_id = shader.m_id;
    shader.m_id = 0;
    m_uniforms = shader.m_uniforms;
}

ShaderSource Shader::parseShader(const std::string& vsPath, const std::string& fsPath)
{
    std::string vertSource;
    std::string fragSource;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    try
    {
        // Open files
        vShaderFile.open(vsPath);
        fShaderFile.open(fsPath);

        std::stringstream vShaderStream, fShaderStream;

        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // Close file handlers
        vShaderFile.close();
        fShaderFile.close();

        // Convert stream into string
        vertSource = vShaderStream.str();
        fragSource = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Failed to read shader source: " << e.what() << "\n";
    }

    return { vertSource, fragSource };
}

bool Shader::compileShader(const ShaderSource& source)
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

void Shader::bind() const
{
    if (m_id != 0)
    {
        glUseProgram(m_id);
    }
}

void Shader::setUniform(std::string name, int value)
{
    glUniform1i(m_uniforms[name].location, value);
}

void Shader::setUniform(std::string name, bool value)
{
    glUniform1i(m_uniforms[name].location, (int)value);
}

void Shader::setUniform(std::string name, float value)
{
    glUniform1f(m_uniforms[name].location, value);
}

void Shader::setUniform(std::string name, const glm::mat4& value)
{
    glUniformMatrix4fv(m_uniforms[name].location, 1, GL_FALSE, glm::value_ptr(value));
}

unsigned int Shader::getId() const
{
    return m_id;
}