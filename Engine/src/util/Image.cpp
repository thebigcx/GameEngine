#include <util/Image.h>

#include <core/Console.h>

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
    int width, height, channels;

    stbi_set_flip_vertically_on_load(m_flipped);

    m_data = stbi_load(file.c_str(), &width, &height, &channels, 4);

    if (!m_data)
    {
        Console::errf("Image does not exist or contains corrupted data: %s", file);
    }

    m_size.x = width;
    m_size.y = height;
    m_channels = channels;
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

int Image::getChannels() const
{
    return m_channels;
}