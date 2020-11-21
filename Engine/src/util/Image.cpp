#include <util/Image.h>

#include <core/Console.h>

#include <stb_image/stb_image.h>

Image::~Image()
{
    stbi_image_free(data);
}

Shared<Image> ImageLoader::loadImage(const std::string& file)
{
    auto image = createShared<Image>();

    int width, height, channels;

    image->data = stbi_load(file.c_str(), &width, &height, &channels, 4);

    if (!image->data)
    {
        std::cout << "Image does not exist or contains corrupted data: " << file << "\n";
    }

    image->width = width;
    image->height = height;
    image->channels = channels;

    return image;
}

Shared<Image> ImageLoader::loadOpenGLImage(const std::string& file)
{
    stbi_set_flip_vertically_on_load(true);

    return loadImage(file);
}