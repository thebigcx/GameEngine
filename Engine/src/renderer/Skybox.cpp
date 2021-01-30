#include <renderer/Skybox.h>

namespace Engine
{

Reference<Skybox> Skybox::create(const std::array<std::string, 6>& files)
{
    Reference<Skybox> skybox = createReference<Skybox>();

    skybox->m_cubemap = TextureCube::create(&files[0]);

    return skybox;
}

}