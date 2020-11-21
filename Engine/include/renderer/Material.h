#pragma once

#include <vector>

#include <core/Core.h>
#include <renderer/shader/Shader.h>
#include <renderer/Texture2D.h>

class Material
{
public:
    void setTexture(const Shared<Texture2D>& texture);

    void bind() const;
    void unbind() const;

    Shared<Shader> getShader() const
    {
        return m_shader;
    }

    static Shared<Material> create(const Shared<Shader>& shader);

private:
    Shared<Shader> m_shader;
    std::vector<Shared<Texture2D>> m_textures;
    //Shared<Texture2D> m_texture;
};