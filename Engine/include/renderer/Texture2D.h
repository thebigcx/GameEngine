#pragma once

#include <GL/glew.h>
#include <stb_image/stb_image.h>

#include <string>

#include <math/vector/Vector2.h>
#include <util/Image.h>
#include <core/Core.h>

class Texture2D
{
public:
    enum class Parameter
    {
        MinFilter = GL_TEXTURE_MIN_FILTER,
        MagFilter = GL_TEXTURE_MAG_FILTER,
        WrapS = GL_TEXTURE_WRAP_S,
        WrapT = GL_TEXTURE_WRAP_T
    };

    enum class Value
    {
        Nearest = GL_NEAREST,
        ClampToEdge = GL_CLAMP_TO_EDGE
    };

    Texture2D() {}

    ~Texture2D();

    Texture2D(Texture2D&& texture);

    static Shared<Texture2D> create(const std::string& file);
    static Shared<Texture2D> create(int width, int height, GLenum dataFormat = GL_RGBA8);

    void updatePixels(float xoffset, float yoffset, float width, float height, const void* data, GLenum dataFormat = GL_RGBA);
    void setParameter(Parameter parameter, Value value);

    void bind(int slot = 0) const;
    void unbind(int slot = 0) const;

    math::Vector2f getSize() const;

    inline unsigned int getId() const
    {
        return m_id;
    }

    // TEMP
    inline void setId(int id)
    {
        m_id = id;
    }

private:
    unsigned int m_id = 0;

    bool m_mipmapped = false;
    GLenum m_internalFormat;
    GLenum m_dataFormat;

    math::Vector2f m_size;
};