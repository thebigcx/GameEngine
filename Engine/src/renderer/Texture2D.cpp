#include <renderer/Texture2D.h>

#include <util/Image.h>
#include <platform/GLTexture2D.h>

#include <iostream>

/*Texture2D::~Texture2D()
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
*/
Shared<Texture2D> Texture2D::create(const std::string& file)
{
    return createShared<GLTexture2D>(file);
}

Shared<Texture2D> Texture2D::create(int width, int height, GLenum dataFormat)
{
    return createShared<GLTexture2D>(width, height, dataFormat);
}
/*
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

math::Vector2f Texture2D::getSize() const
{
    return m_size;
}
*/