#include <renderer/TextureCube.h>
#include <platform/GL/GLTextureCube.h>

namespace Engine
{

Reference<TextureCube> TextureCube::create(const std::string& filepath, bool clamp, bool linear, bool mipmap)
{
    return createReference<GLTextureCube>(filepath, clamp, linear, mipmap);
}

Reference<TextureCube> TextureCube::create(const std::string* files, bool clamp, bool linear, bool mipmap)
{
    return createReference<GLTextureCube>(files, clamp, linear, mipmap);
}

Reference<TextureCube> TextureCube::create(uint32_t width, uint32_t height, SizedTextureFormat internalFormat, bool clamp, bool linear, bool mipmap)
{
    return createReference<GLTextureCube>(width, height, internalFormat, clamp, linear, mipmap);
}

}