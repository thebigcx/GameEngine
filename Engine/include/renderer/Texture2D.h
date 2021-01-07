#pragma once

#include <GL/glew.h>
#include <stb_image/stb_image.h>

#include <string>

#include <maths/vector/vec2.h>
#include <util/ImageLoader.h>
#include <core/Core.h>

namespace Engine
{

class Texture2D
{
public:
    virtual ~Texture2D() = default;

    static Shared<Texture2D> create(const std::string& file, GLenum internalFormat = GL_RGBA8, GLenum dataFormat = GL_RGBA, bool clamp = false, bool linear = true);
    static Shared<Texture2D> create(uint32_t width, uint32_t height, GLenum dataFormat = GL_RGBA8, bool clamp = false, bool linear = true);// TODO: make platform independent (GL_RGBA8)
    static Shared<Texture2D> createWhiteTexture();

    static Shared<Texture2D> asyncCreate(const std::string& file, bool isSRGB = false);

    virtual void setData(uint32_t xoffset, uint32_t yoffset, uint32_t width, uint32_t height, const void* data, GLenum dataFormat = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE) = 0;
    
    virtual void bind(uint32_t slot = 0) const = 0;
    virtual void unbind(uint32_t slot = 0) const = 0;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;

    virtual uint32_t getId() const = 0;

    virtual const std::string& getPath() const = 0;

    virtual bool operator==(const Texture2D& other) = 0;
    virtual bool operator!=(const Texture2D& other) = 0;

private:
    static Shared<Texture2D> s_whiteTexture;
};

}