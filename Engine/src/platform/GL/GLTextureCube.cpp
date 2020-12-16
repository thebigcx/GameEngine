#include <platform/GL/GLTextureCube.h>
#include <util/Image.h>

#include <GL/glew.h>

#include <stb_image/stb_image.h>

GLTextureCube::GLTextureCube(const std::string& filepath)
{
    m_id = loadFromFile(filepath);
}

GLTextureCube::GLTextureCube(const std::string* files)
{
    m_id = loadFromMultipleFiles(files);
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
    unsigned int id = 0;
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &id);
    glBindTextureUnit(0, id);

    return id;
}

uint32_t GLTextureCube::loadFromMultipleFiles(const std::string* files)
{
    unsigned int id = 0;
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &id);
    glBindTextureUnit(0, id);

    for (unsigned int i = 0; i < 6; i++)
    {
        auto image = ImageLoader::loadImage(files[i]);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                     0, 
                     GL_SRGB_ALPHA, 
                     image->width, 
                     image->height, 
                     0, 
                     GL_RGBA, 
                     GL_UNSIGNED_BYTE, 
                     image->data);
    }

    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

    return id;
}