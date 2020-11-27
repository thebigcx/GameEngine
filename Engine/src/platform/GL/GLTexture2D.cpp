#include <platform/GL/GLTexture2D.h>

#include <iostream>

GLTexture2D::GLTexture2D(const std::string& file)
{
    m_path = file;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    bind();

    auto image = ImageLoader::loadOpenGLImage(file);

    if (image->data)
    {
        m_internalFormat = GL_SRGB8_ALPHA8;
        m_dataFormat = GL_RGBA;

        glTextureStorage2D(m_id, 1, m_internalFormat, image->width, image->height);

        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTextureSubImage2D(m_id,
                     0,
                     0,
                     0,
                     image->width,
                     image->height,
                     m_dataFormat,
                     GL_UNSIGNED_BYTE,
                     image->data);

        glGenerateTextureMipmap(m_id);

        m_mipmapped = true;
    }
    else
    {
        std::cout << "Image is corrupted or contains unknown formatted data!\n";
    }

    m_width = image->width;
    m_height = image->height;
}

GLTexture2D::GLTexture2D(uint32_t width, uint32_t height, GLenum dataFormat)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    bind();

    m_internalFormat = dataFormat;
    m_dataFormat = dataFormat;

    glTextureStorage2D(m_id, 1, dataFormat, width, height);
}

GLTexture2D::~GLTexture2D()
{
    glDeleteTextures(1, &m_id);
}

void GLTexture2D::setData(float xoffset, float yoffset, float width, float height, const void* data, GLenum dataFormat)
{
    glTextureSubImage2D(m_id, 0, xoffset, yoffset, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
}

void GLTexture2D::setParameter(Parameter parameter, Value value)
{
    glTextureParameteri(m_id, (GLenum)parameter, (GLenum)value);
}

void GLTexture2D::bind(int slot) const
{
    glBindTextureUnit(slot, m_id);
}

void GLTexture2D::unbind(int slot) const
{
    glBindTextureUnit(slot, 0);
}

float GLTexture2D::getWidth() const
{
    return m_width;
}

float GLTexture2D::getHeight() const
{
    return m_height;
}

unsigned int GLTexture2D::getId() const
{
    return m_id;
}

const std::string& GLTexture2D::getPath() const
{
    return m_path;
}