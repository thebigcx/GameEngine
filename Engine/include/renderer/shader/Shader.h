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
    virtual ~Shader() = default;

    static Shared<Shader> createFromFile(const std::string& vsPath, const std::string& fsPath);
    static Shared<Shader> createFromSource(const std::string& vsSource, const std::string& fsSource);

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void setUniform(const std::string& name, int value) = 0;
    virtual void setUniform(const std::string& name, bool value) = 0;
    virtual void setUniform(const std::string& name, float value) = 0;
    virtual void setUniform(const std::string& name, const math::Vector4f& value) = 0;
    virtual void setUniform(const std::string& name, const Color& value) = 0;
    virtual void setUniform(const std::string& name, const math::Matrix4f& value) = 0;

    virtual unsigned int getId() const = 0;

    enum class DataType
    {
        Float, Bool, Int, Uint, Double,
        Vec2,  Vec3, Vec4,
        iVec2, iVec3, iVec4,
        dVec2, dVec3, dVec4,
        uVec2, uVec3, uVec4,
        bVec2, bVec3, bVec4,
        Mat3,  Mat4,
        dMat3, dMat4,
        Color
    };
};