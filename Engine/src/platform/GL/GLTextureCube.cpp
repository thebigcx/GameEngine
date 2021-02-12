#include <platform/GL/GLTextureCube.h>
#include <util/Image.h>
#include <platform/GL/GLTexture2D.h>

#include <GL/glew.h>

namespace Engine
{

GLTextureCube::GLTextureCube(const std::string& filepath, bool clamp, bool linear, bool mipmap)
{
    m_id = loadFromFile(filepath);

    if (mipmap)
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_R, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);

    if (mipmap)
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
    else
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);

    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
}

GLTextureCube::GLTextureCube(const std::string* files, bool clamp, bool linear, bool mipmap)
{
    m_id = loadFromMultipleFiles(files);

    if (mipmap)
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_R, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);

    if (mipmap)
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
    else
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);

    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
}

GLTextureCube::GLTextureCube(uint32_t width, uint32_t height, SizedTextureFormat internalFormat, bool clamp, bool linear, bool mipmap)
{
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_id);
    glBindTextureUnit(0, m_id);

    glTextureStorage2D(m_id, 5, Utils::getSizedTextureFormatEnumValue_(internalFormat), width, height);

    if (mipmap)
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_R, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);

    if (mipmap)
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
    else
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);

    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
}

void GLTextureCube::generateMipmap() const
{
    glBindTextureUnit(0, m_id);
    glGenerateTextureMipmap(m_id);
    glBindTextureUnit(0, 0);
}

void GLTextureCube::bind(uint32_t slot) const
{
    glBindTextureUnit(slot, m_id);
}

void GLTextureCube::unbind(uint32_t slot) const
{
    glBindTextureUnit(slot, 0);
}

uint32_t GLTextureCube::loadFromFile(const std::string& filepath)
{
    uint32_t id = 0;
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &id);
    glBindTextureUnit(0, id);

    return id;
}

uint32_t GLTextureCube::loadFromMultipleFiles(const std::string* files)
{
    uint32_t id = 0;
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &id);
    glBindTextureUnit(0, id);

    for (unsigned int i = 0; i < 6; i++)
    {
        auto image = Image::create(files[i], true);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                     0, 
                     GL_SRGB_ALPHA, 
                     image->getWidth(), 
                     image->getHeight(), 
                     0, 
                     GL_RGBA, 
                     GL_UNSIGNED_BYTE, 
                     image->getData());
    }

    return id;
}

}