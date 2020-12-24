#pragma once

#include <string>

#include <core/Core.h>

namespace Engine
{

struct Image
{
    ~Image();
    unsigned char* data;
    unsigned int width;
    unsigned int height;
    int channels = 0;
};

class ImageLoader
{
public:
    static Shared<Image> loadOpenGLImage(const std::string& path);
    static Shared<Image> loadImage(const std::string& path);

private:
    static Shared<Image> loadImageImpl(const std::string& path);
};

}