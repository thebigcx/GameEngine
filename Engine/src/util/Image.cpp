#include <util/Image.h>
#include <core/Logger.h>

#include <stb_image/stb_image.h>

namespace Engine
{

Image::Image()
    : m_data(nullptr), m_width(0), m_height(0)
{
    
}

Image::~Image()
{
    stbi_image_free(m_data);
}

Shared<Image> Image::create(const std::string& path, bool flipped)
{
    stbi_set_flip_vertically_on_load(static_cast<int>(flipped));

    int width, height, channels;
    void* data;

    std::string ext = path.substr(path.find_last_of(".") + 1);

    if (ext == "png" || ext == "jpg" || ext == "tga" || ext == "jpeg")
    {
        data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    }
    else if (ext == "hdr")
    {
        data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
    }
    else
    {
        Logger::getCoreLogger()->error("%s has unsupported file format.", path.c_str());
    }

    if (!data)
    {
        Logger::getCoreLogger()->error("Image does not exist or contains corrupted data: %s", path.c_str());
    }

    auto image = Shared<Image>(new Image());

    image->m_data = data;
    image->m_width = width;
    image->m_height = height;
    image->m_channels = channels;
    
    switch (channels)
    {
        case 3:
            image->m_format = Image::Format::RGB;
            break;
        case 4:
            image->m_format = Image::Format::RGBA;
            break;
        default:
            image->m_format = Image::Format::RGBA;
            break;
    };

    return image;
}

}