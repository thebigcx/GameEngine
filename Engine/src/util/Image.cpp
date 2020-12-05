#include <util/Image.h>
#include <core/Logger.h>

#include <stb_image/stb_image.h>

Image::~Image()
{
    stbi_image_free(data);
}

Shared<Image> ImageLoader::loadImageImpl(const std::string& file)
{
    auto image = createShared<Image>();

    int width, height, channels;

    image->data = stbi_load(file.c_str(), &width, &height, &channels, 4);

    if (!image->data)
    {
        Logger::getCoreLogger()->error("Image does not exist or contains corrupted data: %s", file);
    }

    image->width = width;
    image->height = height;
    image->channels = channels;

    return image;
}

Shared<Image> ImageLoader::loadImage(const std::string& file)
{
    stbi_set_flip_vertically_on_load(false);

    return loadImageImpl(file);
}

Shared<Image> ImageLoader::loadOpenGLImage(const std::string& file)
{
    stbi_set_flip_vertically_on_load(true);

    return loadImageImpl(file);
}