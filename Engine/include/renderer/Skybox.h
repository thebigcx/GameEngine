#pragma once

#include <renderer/TextureCube.h>
#include <renderer/shader/Shader.h>

class Skybox
{
public:
    static Shared<Skybox> create(const std::array<std::string, 6>& files);

    Shared<TextureCube> getCubemap() const
    {
        return m_cubemap;
    }

private:
    Shared<TextureCube> m_cubemap;
};