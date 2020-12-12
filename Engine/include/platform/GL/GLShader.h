#pragma once

#include <renderer/shader/Shader.h>

class GLShader : public Shader
{
public:
    GLShader(const std::string& path);
    GLShader(const std::string& vertSource, const std::string& fragSource);
    GLShader(const std::string& path, const std::unordered_map<std::string, std::string>& macros);
    ~GLShader();

    void unbind() const override;
    void bind() const override;

    void setInt(const std::string& name, int value) override;
    void setInt2(const std::string& name, const math::ivec2& value) override;
    void setInt3(const std::string& name, const math::ivec3& value) override;
    void setInt4(const std::string& name, const math::ivec4& value) override;

    void setFloat(const std::string& name, float value) override;
    void setFloat2(const std::string& name, const math::vec2& value) override;
    void setFloat3(const std::string& name, const math::vec3& value) override;
    void setFloat4(const std::string& name, const math::vec4& value) override;

    void setUint(const std::string& name, unsigned int value) override;
    void setUint2(const std::string& name, math::uvec2& value) override;
    void setUint3(const std::string& name, math::uvec3& value) override;
    void setUint4(const std::string& name, math::uvec4& value) override;

    void setFloatArray(const std::string& name, float* value, uint32_t count) override;
    void setFloat2Array(const std::string& name, math::vec2* value, uint32_t count) override;
    void setFloat3Array(const std::string& name, math::vec3* value, uint32_t count) override;
    void setFloat4Array(const std::string& name, math::vec4* value, uint32_t count) override;

    void setIntArray(const std::string& name, int* value, uint32_t count) override;
    void setInt2Array(const std::string& name, math::ivec2* value, uint32_t count) override;
    void setInt3Array(const std::string& name, math::ivec3* value, uint32_t count) override;
    void setInt4Array(const std::string& name, math::ivec4* value, uint32_t count) override;

    void setUintArray(const std::string& name, unsigned int* value, uint32_t count) override;

    void setMatrix4Array(const std::string& name, math::mat4* matrices, uint32_t count) override;

    void setMatrix3(const std::string& name, const math::mat3& value) override;
    void setMatrix4(const std::string& name, const math::mat4& value) override;

    inline unsigned int getId() const override { return m_id; }

private:
    unsigned int m_id = 0;

    ShaderSource preProcess(const std::string& source);
    bool compileShader(const ShaderSource& source);

    std::string processMacros(const std::string& souce, const std::unordered_map<std::string, std::string>& macros);

    std::unordered_map<std::string, uint32_t> m_uniformLocations;

    uint32_t getUniformLocation(const std::string& uniform);
};