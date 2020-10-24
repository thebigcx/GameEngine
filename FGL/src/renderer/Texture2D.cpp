#include <renderer/Texture2D.h>

#include <util/Image.h>
#include <util/Timer.h>

#include <iostream>

Texture2D::Texture2D()
{
    glGenTextures(1, &m_id);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_id);
}

Texture2D::Texture2D(const Texture2D& texture)
{
    m_id = texture.m_id;
    m_flipped = texture.m_flipped;
    m_mipmapped = texture.m_mipmapped;
    m_size = texture.m_size;
    m_smooth = texture.m_smooth;
    m_repeated = texture.m_repeated;
}

Texture2D::Texture2D(Texture2D&& texture)
{
    m_id = texture.m_id;
    texture.m_id = 0;
    m_flipped = texture.m_flipped;
    m_mipmapped = texture.m_mipmapped;
    m_size = texture.m_size;
    m_smooth = texture.m_smooth;
    m_repeated = texture.m_repeated;
}

bool Texture2D::loadFile(const std::string& file)
{
    glBindTexture(GL_TEXTURE_2D, m_id);

    Image image;
    image.setVerticalFlip(m_flipped);
    image.loadFile(file);

    return loadImage(image);
}

bool Texture2D::loadImage(const Image& image)
{
    if (image.getPixels())
    {
        glTexImage2D(GL_TEXTURE_2D,      // Type of texture
                     0,                  // Level of detail (0 default)
                     GL_RGBA,            // Format of texture
                     image.getSize().x,  // Height
                     image.getSize().y,  // Width
                     0,                  // Border (must be 0)
                     GL_RGBA,            // Format of image
                     GL_UNSIGNED_BYTE,   // Type of pixel data (uint8_t)
                     image.getPixels()); // Pixel data

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        m_mipmapped = true;
    }
    else
    {
        std::cout << "Image is corrupted or contains unknown formatted data!\n";
        return false;
    }

    m_size.x = image.getSize().x;
    m_size.y = image.getSize().y;

    return true;
}

void Texture2D::create(int width, int height)
{
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

void Texture2D::setSmooth(bool smooth)
{
    bind();

    auto filter = smooth ? GL_LINEAR : GL_NEAREST;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    m_smooth = smooth;
}

bool Texture2D::isSmooth() const
{
    return m_smooth;
}

void Texture2D::setRepeated(bool repeated)
{
    bind();

    auto repeat = repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat);

    m_repeated = repeated;
}

bool Texture2D::isRepeated() const
{
    return m_repeated;
}

void Texture2D::setVerticalFlip(bool flip)
{
    m_flipped = flip;
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

Vector2u Texture2D::getSize() const
{
    return m_size;
}