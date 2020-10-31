#include <renderer/Texture2D.h>

#include <util/Image.h>

#include <iostream>

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_id);
}

Texture2D::Texture2D(Texture2D&& texture)
{
    m_id = texture.m_id;
    texture.m_id = 0;
    m_mipmapped = texture.m_mipmapped;
    m_size = texture.m_size;
}

Shared<Texture2D> Texture2D::create(const std::string& file)
{
    auto texture = createShared<Texture2D>();
    glCreateTextures(GL_TEXTURE_2D, 1, &(texture->m_id));
    texture->bind();

    Image image;
    image.setVerticalFlip(true);
    image.loadFile(file);

    if (image.getPixels())
    {
        GLenum internalFormat, dataFormat;
        if (image.getChannels() == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (image.getChannels() == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        texture->m_internalFormat = internalFormat;
        texture->m_dataFormat = dataFormat;

        glTextureStorage2D(texture->m_id, 1, internalFormat, image.getSize().x, image.getSize().y);

        glTextureParameteri(texture->m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(texture->m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

        glTextureParameteri(texture->m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(texture->m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTextureSubImage2D(texture->m_id,
                     0,
                     0,
                     0,
                     image.getSize().x,
                     image.getSize().y,
                     dataFormat,
                     GL_UNSIGNED_BYTE,
                     image.getPixels());

        glGenerateTextureMipmap(texture->m_id);

        texture->m_mipmapped = true;
    }
    else
    {
        std::cout << "Image is corrupted or contains unknown formatted data!\n";
    }

    texture->m_size.x = image.getSize().x;
    texture->m_size.y = image.getSize().y;

    return texture;
}

Shared<Texture2D> Texture2D::create(int width, int height, GLenum dataFormat)
{
    auto texture = createShared<Texture2D>();
    glCreateTextures(GL_TEXTURE_2D, 1, &(texture->m_id));
    texture->bind();

    texture->m_internalFormat = dataFormat;
    texture->m_dataFormat = dataFormat;

    glTextureStorage2D(texture->m_id, 1, dataFormat, width, height);

    return texture;
}

void Texture2D::updatePixels(float xoffset, float yoffset, float width, float height, const void* data, GLenum dataFormat)
{
    glTextureSubImage2D(m_id, 0, xoffset, yoffset, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
}

void Texture2D::setParameter(Parameter parameter, Value value)
{
    glTextureParameteri(m_id, (GLenum)parameter, (GLenum)value);
}

void Texture2D::bind(int slot) const
{
    glBindTextureUnit(slot, m_id);
}

void Texture2D::unbind(int slot) const
{
    glBindTextureUnit(slot, 0);
}

Vector2f Texture2D::getSize() const
{
    return m_size;
}