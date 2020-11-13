#pragma once

#include <renderer/shader/Shader.h>

class GLShader : public Shader
{
public:
    GLShader(const std::string& path);
    GLShader(const std::string& vertSource, const std::string& fragSource);
    ~GLShader();

    void unbind() const override;
    void bind() const override;

    void setInt(const std::string& name, int value) override;
    void setInt2(const std::string& name, const math::Vector2i& value) override;
    void setInt3(const std::string& name, const math::Vector3i& value) override;
    void setInt4(const std::string& name, const math::Vector4i& value) override;

    void setFloat(const std::string& name, float value) override;
    void setFloat2(const std::string& name, const math::Vector2f& value) override;
    void setFloat3(const std::string& name, const math::Vector3f& value) override;
    void setFloat4(const std::string& name, const math::Vector4f& value) override;

    void setUint(const std::string& name, unsigned int value) override;
    void setUint2(const std::string& name, math::Vector2u& value) override;
    void setUint3(const std::string& name, math::Vector3u& value) override;
    void setUint4(const std::string& name, math::Vector4u& value) override;

    void setFloatArray(const std::string& name, float* value, uint32_t count) override;
    void setFloat2Array(const std::string& name, math::Vector2f* value, uint32_t count) override;
    void setFloat3Array(const std::string& name, math::Vector3f* value, uint32_t count) override;
    void setFloat4Array(const std::string& name, math::Vector4f* value, uint32_t count) override;

    void setIntArray(const std::string& name, int* value, uint32_t count) override;
    void setInt2Array(const std::string& name, math::Vector2i* value, uint32_t count) override;
    void setInt3Array(const std::string& name, math::Vector3i* value, uint32_t count) override;
    void setInt4Array(const std::string& name, math::Vector4i* value, uint32_t count) override;

    void setUintArray(const std::string& name, unsigned int* value, uint32_t count) override;

    void setMatrix3(const std::string& name, const math::Matrix3f& value) override;
    void setMatrix4(const std::string& name, const math::Matrix4f& value) override;

    inline unsigned int getId() const override { return m_id; }

private:
    unsigned int m_id = 0;

    ShaderSource preProcess(const std::string& source);
    bool compileShader(const ShaderSource& source);
};