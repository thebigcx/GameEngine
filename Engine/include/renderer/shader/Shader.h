#pragma once

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include <maths/vector/vec4.h>
#include <maths/matrix/mat4.h>
#include <maths/matrix/mat3.h>
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

    static Shared<Shader> createFromFile(const std::string& path);
    static Shared<Shader> createFromSource(const std::string& vertSource, const std::string& fragSource);

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void setInt(const std::string& name, int value) = 0;
    virtual void setInt2(const std::string& name, const math::ivec2& value) = 0;
    virtual void setInt3(const std::string& name, const math::ivec3& value) = 0;
    virtual void setInt4(const std::string& name, const math::ivec4& value) = 0;

    virtual void setFloat(const std::string& name, float value) = 0;
    virtual void setFloat2(const std::string& name, const math::vec2& value) = 0;
    virtual void setFloat3(const std::string& name, const math::vec3& value) = 0;
    virtual void setFloat4(const std::string& name, const math::vec4& value) = 0;

    virtual void setUint(const std::string& name, unsigned int value) = 0;
    virtual void setUint2(const std::string& name, math::uvec2& value) = 0;
    virtual void setUint3(const std::string& name, math::uvec3& value) = 0;
    virtual void setUint4(const std::string& name, math::uvec4& value) = 0;

    virtual void setFloatArray(const std::string& name, float* value, uint32_t count) = 0;
    virtual void setFloat2Array(const std::string& name, math::vec2* value, uint32_t count) = 0;
    virtual void setFloat3Array(const std::string& name, math::vec3* value, uint32_t count) = 0;
    virtual void setFloat4Array(const std::string& name, math::vec4* value, uint32_t count) = 0;

    virtual void setIntArray(const std::string& name, int* value, uint32_t count) = 0;
    virtual void setInt2Array(const std::string& name, math::ivec2* value, uint32_t count) = 0;
    virtual void setInt3Array(const std::string& name, math::ivec3* value, uint32_t count) = 0;
    virtual void setInt4Array(const std::string& name, math::ivec4* value, uint32_t count) = 0;

    virtual void setUintArray(const std::string& name, unsigned int* value, uint32_t count) = 0;

    virtual void setMatrix3(const std::string& name, const math::mat3& value) = 0;
    virtual void setMatrix4(const std::string& name, const math::mat4& value) = 0;

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
        dMat3, dMat4
    };
};