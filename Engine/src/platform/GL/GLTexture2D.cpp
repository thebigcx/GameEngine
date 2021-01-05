#include <platform/GL/GLTexture2D.h>
#include <core/Logger.h>

#include <iostream>

namespace Engine
{

GLTexture2D::GLTexture2D(const std::string& file, GLenum internalFormat, GLenum dataFormat, bool clamp, bool linear)
    : m_path(file)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    bind();

    auto image = ImageLoader::loadOpenGLImage(file);

    if (image->data)
    {
        m_internalFormat = internalFormat;
        m_dataFormat = dataFormat;

        glTextureStorage2D(m_id, 1, m_internalFormat, image->width, image->height);

        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);

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
    }
    else
    {
        Logger::getCoreLogger()->error("Image is corrupted or contains unknown formatted data!");
    }

    m_width = image->width;
    m_height = image->height;

    unbind();
}

GLTexture2D::GLTexture2D(uint32_t width, uint32_t height, GLenum dataFormat, bool clamp, bool linear)
    : m_internalFormat(dataFormat), m_dataFormat(dataFormat), m_path("")
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);

    bind();

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);

    glTextureStorage2D(m_id, 1, dataFormat, width, height);
}

GLTexture2D::~GLTexture2D()
{
    glDeleteTextures(1, &m_id);
}

void GLTexture2D::setData(uint32_t xoffset, uint32_t yoffset, uint32_t width, uint32_t height, const void* data, GLenum dataFormat, GLenum type)
{
    bind();

    glTextureSubImage2D(m_id, 0, xoffset, yoffset, width, height, dataFormat, type, data);
    
    unbind();
}

void GLTexture2D::bind(uint32_t slot) const
{
    glBindTextureUnit(slot, m_id);
}

void GLTexture2D::unbind(uint32_t slot) const
{
    glBindTextureUnit(slot, 0);
}

bool GLTexture2D::operator==(const Texture2D& other)
{
    return m_id == static_cast<const GLTexture2D&>(other).m_id;
}

bool GLTexture2D::operator!=(const Texture2D& other)
{
    return m_id != static_cast<const GLTexture2D&>(other).m_id;
}

}