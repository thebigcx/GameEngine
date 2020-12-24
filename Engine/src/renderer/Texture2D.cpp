#include <renderer/Texture2D.h>

#include <util/Image.h>
#include <platform/GL/GLTexture2D.h>

#include <iostream>
#include <thread>

Shared<Texture2D> Texture2D::create(const std::string& file, bool isSRGB)
{
    return createShared<GLTexture2D>(file, isSRGB);
}

Shared<Texture2D> Texture2D::create(int width, int height, GLenum dataFormat)
{
    return createShared<GLTexture2D>(width, height, dataFormat);
}

Shared<Texture2D> Texture2D::createWhiteTexture()
{
    auto texture = createShared<GLTexture2D>(1, 1);
    uint32_t white = 0xffffffff;
    texture->setData(0, 0, 1, 1, &white);
    return texture;
}

Shared<Texture2D> Texture2D::asyncCreate(const std::string& file, bool isSRGB)
{
    Shared<Image> img;
    std::thread thr([&]
    {
        img = ImageLoader::loadOpenGLImage(file);
    });
    

    Shared<Texture2D> texture = createShared<GLTexture2D>(img->width, img->height);
    texture->setData(0, 0, img->width, img->height, img->data);
}