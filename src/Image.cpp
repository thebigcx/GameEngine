#include "FGL/Image.h"

#include <stb_image/stb_image.h>

Image::Image()
{

}

Image::~Image()
{
    stbi_image_free(m_data);
}

void Image::loadFile(const std::string& file)
{
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(m_flipped);

    m_data = stbi_load(file.c_str(), &width, &height, &nrChannels, 4);

    m_size.x = width;
    m_size.y = height;
}

unsigned char* Image::getPixels() const
{
    return m_data;
}

const Vector2u& Image::getSize() const
{
    return m_size;
}

void Image::setVerticalFlip(bool flip)
{
    m_flipped = flip;
}