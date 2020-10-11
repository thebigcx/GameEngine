#include "renderer/Texture.h"

#include "../Image.h"

#include <iostream>

Texture::Texture()
{
    glGenTextures(1, &m_id);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

bool Texture::loadFile(const std::string& file)
{
    glBindTexture(GL_TEXTURE_2D, m_id);

    Image image;
    image.loadFile(file);

    if (image.getPixels())
    {
        glTexImage2D(GL_TEXTURE_2D,      // Type of texture
                     0,                  // Level of detail (0 default)
                     GL_RGB,             // Format of texture
                     image.getSize().x,  // Height
                     image.getSize().y,  // Width
                     0,                  // Border (must be 0)
                     GL_RGB,             // Format of image
                     GL_UNSIGNED_BYTE,   // Type of pixel data (uint8_t)
                     image.getPixels()); // Pixel data

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        m_mipmapped = true;
    }
    else
    {
        std::cout << "Could not load texture at: " << file << "\n";
        return false;
    }

    m_size.x = image.getSize().x;
    m_size.y = image.getSize().y;

    return true;
}

void Texture::setSmooth(bool smooth)
{
    bind();

    auto filter = smooth ? GL_NEAREST : GL_LINEAR;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    m_smooth = smooth;
}

bool Texture::isSmooth() const
{
    return m_smooth;
}

void Texture::setRepeated(bool repeated)
{
    bind();

    auto repeat = repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat);

    m_repeated = repeated;
}

bool Texture::isRepeated() const
{
    return m_repeated;
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

Vector2u Texture::getSize() const
{
    return m_size;
}