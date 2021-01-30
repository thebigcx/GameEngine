#pragma once

#include <renderer/TextureCube.h>
#include <renderer/shader/Shader.h>

namespace Engine
{

class Skybox
{
public:
    static Reference<Skybox> create(const std::array<std::string, 6>& files);

    Reference<TextureCube> getCubemap() const
    {
        return m_cubemap;
    }

private:
    Reference<TextureCube> m_cubemap;
};

}