#include <renderer/TextureCube.h>
#include <platform/GL/GLTextureCube.h>

namespace Engine
{

Shared<TextureCube> TextureCube::create(const std::string& filepath, bool clamp, bool linear, bool mipmap)
{
    return createShared<GLTextureCube>(filepath, clamp, linear, mipmap);
}

Shared<TextureCube> TextureCube::create(const std::string* files, bool clamp, bool linear, bool mipmap)
{
    return createShared<GLTextureCube>(files, clamp, linear, mipmap);
}

Shared<TextureCube> TextureCube::create(uint32_t width, uint32_t height, SizedTextureFormat internalFormat, bool clamp, bool linear, bool mipmap)
{
    return createShared<GLTextureCube>(width, height, internalFormat, clamp, linear, mipmap);
}

}