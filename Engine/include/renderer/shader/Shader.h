#pragma once

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include <math/vector/Vector4.h>
#include <math/matrix/Matrix4.h>
#include <util/Color.h>
#include <core/Core.h>

struct ShaderSource
{
    std::string vertex;
    std::string fragment;
};

struct Uniform
{
    size_t size;
    GLenum type;
    int location;
};

class Shader
{
public:
    Shader();
    ~Shader();

    Shader(const Shader& shader);

    static Shared<Shader> createFromFile(const std::string& vsPath, const std::string& fsPath);
    static Shared<Shader> createFromSource(const std::string& vsSource, const std::string& fsSource);

    void bind() const;
    void unbind() const;

    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, bool value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, const Vector4f& value);
    void setUniform(const std::string& name, const Color& value);
    void setUniform(const std::string& name, const Matrix4f& value);

    unsigned int getId() const;

    enum class DataType
    {
        Float, Bool, Int,
        Vec2,  Vec3,
        Mat3,  Mat4,
        Vec2i, Vec3i,
        Color
    };

private:
    unsigned int m_id = 0;

    static ShaderSource parseShader(const std::string& vsPath, const std::string& fsPath);

    bool compileShader(const ShaderSource& source);
};