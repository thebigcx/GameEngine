#include <renderer/Texture2D.h>

#include <util/Image.h>
#include <platform/GL/GLTexture2D.h>

#include <iostream>
#include <thread>

namespace Engine
{

Shared<Texture2D> Texture2D::s_whiteTexture = nullptr;

Shared<Texture2D> Texture2D::create(const std::string& file, bool clamp, bool linear)
{
    return createShared<GLTexture2D>(file, clamp, linear);
}

Shared<Texture2D> Texture2D::create(uint32_t width, uint32_t height, SizedTextureFormat dataFormat, bool clamp, bool linear)
{
    return createShared<GLTexture2D>(width, height, dataFormat, clamp, linear);
}

Shared<Texture2D> Texture2D::createWhiteTexture()
{
    if (!s_whiteTexture)
    {
        s_whiteTexture = createShared<GLTexture2D>(1, 1);
        uint32_t white = 0xffffffff;
        s_whiteTexture->setData(0, 0, 1, 1, &white);
    }

    return s_whiteTexture;
}

Shared<Texture2D> Texture2D::asyncCreate(const std::string& file, bool isSRGB)
{
    Shared<Image> img;
    std::thread thr([&]
    {
        img = Image::create(file, true);
    });
    

    Shared<Texture2D> texture = createShared<GLTexture2D>(img->getWidth(), img->getHeight());
    texture->setData(0, 0, img->getWidth(), img->getHeight(), img->getData());
}

}