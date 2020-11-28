#include <renderer/TextureCube.h>
#include <platform/GL/GLTextureCube.h>

Shared<TextureCube> TextureCube::create(const std::string& filepath)
{
    return createShared<GLTextureCube>(filepath);
}

Shared<TextureCube> TextureCube::create(const std::string* files)
{
    return createShared<GLTextureCube>(files);
}