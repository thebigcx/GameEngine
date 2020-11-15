#include <renderer/Texture2D.h>

#include <util/Image.h>
#include <platform/GL/GLTexture2D.h>

#include <iostream>

Shared<Texture2D> Texture2D::create(const std::string& file)
{
    return createShared<GLTexture2D>(file);
}

Shared<Texture2D> Texture2D::create(int width, int height, GLenum dataFormat)
{
    return createShared<GLTexture2D>(width, height, dataFormat);
}