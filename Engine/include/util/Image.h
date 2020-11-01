#pragma once

#include <string>

#include <math/vector/Vector2.h>
#include <core/Core.h>

struct Image
{
    ~Image();
    unsigned char* data;
    Vector2u size;
    int channels = 0;
};

class ImageLoader
{
public:
    static Shared<Image> loadOpenGLImage(const std::string& path);
    static Shared<Image> loadImage(const std::string& path);
};