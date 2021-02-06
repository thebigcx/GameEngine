#pragma once

#include <string>

#include <maths/vector/vec2.h>
#include <core/Core.h>

namespace Engine
{

enum class SizedTextureFormat
{
    None = 0,
    RGBA8,
    RGB8,
    Depth24Stencil8,
    Depth16,
    RGB16F,
    RGBA16F,
    sRGBA8,
    sRGB8,

    Depth = Depth24Stencil8
};

enum class TextureFormat
{
    None = 0,
    RGB,
    RGBA,
    Red,
    RG,
    BGR,
    BGRA,
    IntegerRed,
    IntegerRG,
    IntegerRGB,
    IntegerBGR,
    IntegerRGBA,
    IntegerBGRA,
    StencilIndex,
    DepthComponent,
    DepthStencil
};

enum class DataType
{
    UnsignedByte,
    Byte,
    UnsignedShort,
    Short,
    UnsignedInt,
    Int,
    HalfFloat,
    Float
};

class Texture2D
{
public:
    virtual ~Texture2D() = default;

    static Reference<Texture2D> create(const Reference<Texture2D>& other);
    static Reference<Texture2D> create(const std::string& file, bool clamp = false, bool linear = true, bool isSRGB = true);
    static Reference<Texture2D> create(uint32_t width, uint32_t height, SizedTextureFormat dataFormat = SizedTextureFormat::RGBA8, bool clamp = false, bool linear = true);
    static Reference<Texture2D> createWhiteTexture();

    static Reference<Texture2D> asyncCreate(const std::string& file, bool isSRGB = false);

    virtual void setData(uint32_t xoffset, uint32_t yoffset, uint32_t width, uint32_t height, const void* data, 
                         TextureFormat dataFormat = TextureFormat::RGBA, DataType type = DataType::UnsignedByte) = 0;
    
    virtual void bind(uint32_t slot = 0) const = 0;
    virtual void unbind(uint32_t slot = 0) const = 0;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;

    virtual bool isClamped() const = 0;
    virtual bool isLinear() const = 0;
    virtual bool isSRGB() const = 0;

    virtual uint32_t getId() const = 0;

    virtual const std::string& getPath() const = 0;

    virtual bool operator==(const Texture2D& other) = 0;
    virtual bool operator!=(const Texture2D& other) = 0;

    std::string name = "";
    std::string uuid = "";

private:
    static Reference<Texture2D> s_whiteTexture;
};

}