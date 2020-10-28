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
    m_smooth = texture.m_smooth;
    m_repeated = texture.m_repeated;
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

Shared<Texture2D> Texture2D::create(int width, int height)
{
    auto texture = createShared<Texture2D>();
    glCreateTextures(GL_TEXTURE_2D, 1, &(texture->m_id));
    texture->bind();

    texture->m_internalFormat = GL_RGBA8;
    texture->m_dataFormat = GL_RGBA;

    glTextureStorage2D(texture->m_id, 1, GL_RGBA8, width, height);

    return texture;
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

void Texture2D::bind() const
{
    glBindTextureUnit(0, m_id);
}

Vector2f Texture2D::getSize() const
{
    return m_size;
}