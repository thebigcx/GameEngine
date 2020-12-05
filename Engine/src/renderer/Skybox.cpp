#include <renderer/Skybox.h>

Shared<Skybox> Skybox::create(const std::array<std::string, 6>& files)
{
    Shared<Skybox> skybox = createShared<Skybox>();

    skybox->m_cubemap = TextureCube::create(&files[0]);

    return skybox;
}