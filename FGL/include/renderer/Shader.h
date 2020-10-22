#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

struct ShaderSource
{
    std::string vertex;
    std::string fragment;
};

struct Uniform
{
    std::string name;
    size_t size;
    GLenum type;
    int location;
};

class Shader
{
public:
    Shader();
    ~Shader();

    Shader(const std::string& vsPath, const std::string& fsPath);

    Shader(const Shader& shader);
    Shader(Shader&& shader);

    void create(const std::string& vsPath, const std::string& fsPath);

    void bind() const;

    void setUniform(std::string name, int value);
    void setUniform(std::string name, bool value);
    void setUniform(std::string name, float value);
    void setUniform(std::string name, const glm::mat4& value);

    unsigned int getId() const;

    enum DataType
    {
        Float,
        Vec2,
        Vec3,
        Mat3,
        Mat4,
        Bool,
        Int,
        Vec2i,
        Vec3i,
        Color
    };

private:
    unsigned int m_id;

    static ShaderSource parseShader(const std::string& vsPath, const std::string& fsPath);

    bool compileShader(const ShaderSource& source);

    std::unordered_map<std::string, Uniform> m_uniforms;
};