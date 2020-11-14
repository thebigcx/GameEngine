#pragma once

#include <string>

#include <math/vector/vec2.h>
#include <core/Core.h>

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
};