#pragma once

#include <string>

#include <core/Core.h>
#include <renderer/Texture2D.h>

#include <GL/glew.h>

namespace Engine
{

class TextureCube
{
public:
    virtual ~TextureCube() = default;

    virtual void bind(uint32_t slot = 0) const = 0;
    virtual void unbind(uint32_t slot = 0) const = 0;
    
    virtual bool operator==(const TextureCube& array) const = 0;
    virtual bool operator!=(const TextureCube& array) const = 0;

    virtual uint32_t getId() const = 0;

    static Shared<TextureCube> create(const std::string& filepath, bool clamp = false, bool linear = true, bool mipmap = false);
    static Shared<TextureCube> create(const std::string* files, bool clamp = false, bool linear = true, bool mipmap = false);
    static Shared<TextureCube> create(uint32_t width, uint32_t height, SizedTextureFormat dataFormat = SizedTextureFormat::RGBA8, bool clamp = false, bool linear = true, bool mipmap = false);

    enum class Face
    {
        PositiveX, NegativeX,
        PositiveY, NegativeY,
        PositiveZ, NegativeZ
    };

};

}