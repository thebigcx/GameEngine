#pragma once

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include <maths/vector/vec4.h>
#include <maths/matrix/mat4x4.h>
#include <maths/matrix/mat3x3.h>
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
    static Shared<Shader> createFromFileWithMacros(const std::string& path, const std::unordered_map<std::string, std::string>& macros);

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

    virtual void setMatrix4Array(const std::string& name, math::mat4* matrices, uint32_t count) = 0;

    virtual void setMatrix3(const std::string& name, const math::mat3& value) = 0;
    virtual void setMatrix4(const std::string& name, const math::mat4& value) = 0;

    virtual unsigned int getId() const = 0;

    enum class DataType
    {
        Float,  Float2,  Float3,  Float4,
        Int,    Int2,    Int3,    Int4,
        UInt,   UInt2,   UInt3,   UInt4,
        Bool,   Bool2,   Bool3,   Bool4,
        Double, Double2, Double3, Double4,

        Mat2, Mat3, Mat4,
        Mat2x2, Mat2x3, Mat2x4, 
        Mat3x2, Mat3x3, Mat3x4, 
        Mat4x2, Mat4x3, Mat4x4,

        DMat2, DMat3, DMat4,
        DMat2x2, DMat2x3, DMat2x4, 
        DMat3x2, DMat3x3, DMat3x4, 
        DMat4x2, DMat4x3, DMat4x4
    };
};