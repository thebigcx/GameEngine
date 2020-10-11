#include "Shader.h"

Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
    std::string vertSource;
    std::string fragSource;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    try
    {
        // Open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

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
        printf("Failed to read shader source.\n");
    }

    const char* vShaderCode = vertSource.c_str();
    const char* fShaderCode = fragSource.c_str();

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
        std::cout << "Failed to link shaders program.\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::bind()
{
    glUseProgram(m_id);
}

void Shader::setUniform(std::string p_name, int p_value)
{
    glUniform1i(glGetUniformLocation(m_id, p_name.c_str()), p_value);
}

void Shader::setUniform(std::string p_name, bool p_value)
{
    glUniform1i(glGetUniformLocation(m_id, p_name.c_str()), (int)p_value);
}

void Shader::setUniform(std::string p_name, float p_value)
{
    glUniform1f(glGetUniformLocation(m_id, p_name.c_str()), p_value);
}

void Shader::setUniform(std::string p_name, glm::mat4 p_value)
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, p_name.c_str()), 1, GL_FALSE, glm::value_ptr(p_value));
}

unsigned int Shader::getId() const
{
    return m_id;
}