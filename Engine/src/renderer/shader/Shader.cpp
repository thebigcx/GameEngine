#include <vector>

#include <renderer/shader/Shader.h>
#include <util/Timer.h>

Shader::Shader()
{

}

Shared<Shader> Shader::createFromFile(const std::string& vsPath, const std::string& fsPath)
{
    auto shader = createShared<Shader>();

    ShaderSource source = parseShader(vsPath, fsPath);
    shader->compileShader(source);

    return shader;
}

Shared<Shader> Shader::createFromSource(const std::string& vsSource, const std::string& fsSource)
{
    auto shader = createShared<Shader>();

    ShaderSource source = { vsSource, fsSource };
    shader->compileShader(source);
    
    return shader;
}

Shader::~Shader()
{
    if (m_id != 0)
    {
        glDeleteProgram(m_id);
    }
}

Shader::Shader(const Shader& shader)
{
    m_id = shader.getId();
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

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::setUniform(const std::string& name, int value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform1i(location, value);
}

void Shader::setUniform(const std::string& name, bool value)
{
    setUniform(name, static_cast<int>(value));
}

void Shader::setUniform(const std::string& name, float value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform1f(location, value);
}

void Shader::setUniform(const std::string& name, const math::Vector4f& value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::setUniform(const std::string& name, const Color& value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform4f(location, value.r, value.g, value.b, value.a);
}

void Shader::setUniform(const std::string& name, const math::Matrix4f& value)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, math::buffer(value));
}

unsigned int Shader::getId() const
{
    return m_id;
}