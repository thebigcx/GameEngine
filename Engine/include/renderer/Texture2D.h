#pragma once

#include <GL/glew.h>
#include <stb_image/stb_image.h>

#include <string>

#include <maths/vector/vec2.h>
#include <util/Image.h>
#include <core/Core.h>

namespace Engine
{

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

    virtual ~Texture2D() = default;

    static Shared<Texture2D> create(const std::string& file, bool isSRGB = false, bool clamp = false, bool linear = true);
    static Shared<Texture2D> create(int width, int height, GLenum dataFormat = GL_RGBA8, bool clamp = false, bool linear = true);// TODO: make platform independent (GL_RGBA8)
    static Shared<Texture2D> createWhiteTexture();

    static Shared<Texture2D> asyncCreate(const std::string& file, bool isSRGB = false);

    virtual void setData(float xoffset, float yoffset, float width, float height, const void* data, GLenum dataFormat = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE) = 0;
    virtual void setParameter(Parameter parameter, Value value) = 0;

    virtual void bind(uint32_t slot = 0) const = 0;
    virtual void unbind(uint32_t slot = 0) const = 0;

    virtual float getWidth() const = 0;
    virtual float getHeight() const = 0;

    virtual unsigned int getId() const = 0;

    virtual const std::string& getPath() const = 0;

    virtual bool operator==(const Texture2D& other) = 0;
    virtual bool operator!=(const Texture2D& other) = 0;

private:
    static Shared<Texture2D> s_whiteTexture;
};

}