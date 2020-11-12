#pragma once

#include <renderer/shader/Shader.h>

class GLShader : public Shader
{
public:
    GLShader(const std::string& vert, const std::string& frag);
    ~GLShader();

    void unbind() const override;
    void bind() const override;

    void setUniform(const std::string& name, int value) override;
    void setUniform(const std::string& name, bool value) override;
    void setUniform(const std::string& name, float value) override;
    void setUniform(const std::string& name, const math::Vector4f& value) override;
    void setUniform(const std::string& name, const Color& value) override;
    void setUniform(const std::string& name, const math::Matrix4f& value) override;

    inline unsigned int getId() const override { return m_id; }

private:
    unsigned int m_id = 0;

    static ShaderSource parseShader(const std::string& vsPath, const std::string& fsPath);

    bool compileShader(const ShaderSource& source);
};