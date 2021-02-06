#include <renderer/Texture2D.h>

#include <util/Image.h>
#include <platform/GL/GLTexture2D.h>

#include <iostream>
#include <thread>

namespace Engine
{

Reference<Texture2D> Texture2D::s_whiteTexture = nullptr;

Reference<Texture2D> Texture2D::create(const Reference<Texture2D>& other)
{
    return createReference<GLTexture2D>(static_cast<const GLTexture2D&>(*other));
}

Reference<Texture2D> Texture2D::create(const std::string& file, bool clamp, bool linear, bool isSRGB)
{
    return createReference<GLTexture2D>(file, clamp, linear, isSRGB);
}

Reference<Texture2D> Texture2D::create(uint32_t width, uint32_t height, SizedTextureFormat dataFormat, bool clamp, bool linear)
{
    return createReference<GLTexture2D>(width, height, dataFormat, clamp, linear);
}

Reference<Texture2D> Texture2D::createWhiteTexture()
{
    if (!s_whiteTexture)
    {
        s_whiteTexture = createReference<GLTexture2D>(1, 1);
        uint32_t white = 0xffffffff;
        s_whiteTexture->setData(0, 0, 1, 1, &white);
    }

    return s_whiteTexture;
}

Reference<Texture2D> Texture2D::asyncCreate(const std::string& file, bool isSRGB)
{
    Reference<Image> img;
    std::thread thr([&]
    {
        img = Image::create(file, true);
    });
    

    Reference<Texture2D> texture = createReference<GLTexture2D>(img->getWidth(), img->getHeight());
    texture->setData(0, 0, img->getWidth(), img->getHeight(), img->getData());
}

}