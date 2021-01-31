#include <platform/GL/GLTexture2D.h>
#include <core/Logger.h>
#include <util/Image.h>

#include <GL/glew.h>

#include <iostream>

namespace Engine
{

namespace Utils
{
    uint32_t getSizedTextureFormatEnumValue_(SizedTextureFormat format)
    {
        switch (format)
        {
            case SizedTextureFormat::RGBA8: return GL_RGBA8;
            case SizedTextureFormat::RGB8: return GL_RGB8;
            case SizedTextureFormat::Depth16: return GL_DEPTH_COMPONENT16;
            case SizedTextureFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
            case SizedTextureFormat::RGB16F: return GL_RGB16F;
            case SizedTextureFormat::sRGB8: return GL_SRGB8;
            case SizedTextureFormat::sRGBA8: return GL_SRGB8_ALPHA8;
        };
        return 0;
    }

    uint32_t getTextureFormatEnumValue_(TextureFormat format)
    {
        switch (format)
        {
            case TextureFormat::RG: return GL_RG;
            case TextureFormat::RGB: return GL_RGB;
            case TextureFormat::RGBA: return GL_RGBA;
            case TextureFormat::Red: return GL_RED;
            case TextureFormat::BGR: return GL_BGR;
            case TextureFormat::BGRA: return GL_BGRA;
            case TextureFormat::IntegerRed: return GL_RED_INTEGER;
            case TextureFormat::IntegerRG: return GL_RG_INTEGER;
            case TextureFormat::IntegerRGB: return GL_RGB_INTEGER;
            case TextureFormat::IntegerBGR: return GL_BGR_INTEGER;
            case TextureFormat::IntegerRGBA: return GL_RGBA_INTEGER;
            case TextureFormat::IntegerBGRA: return GL_BGRA_INTEGER;
            case TextureFormat::StencilIndex: return GL_STENCIL_INDEX;
            case TextureFormat::DepthComponent: return GL_DEPTH_COMPONENT;
            case TextureFormat::DepthStencil: return GL_DEPTH_STENCIL;
        };
        return 0;
    }

    uint32_t getDataTypeEnumValue_(DataType type)
    {
        switch (type)
        {
            case DataType::UnsignedByte: return GL_UNSIGNED_BYTE;
            case DataType::Byte: return GL_BYTE;
            case DataType::UnsignedShort: return GL_UNSIGNED_SHORT;
            case DataType::Short: return GL_SHORT;
            case DataType::UnsignedInt: return GL_UNSIGNED_INT;
            case DataType::Int: return GL_INT;
            case DataType::HalfFloat: return GL_HALF_FLOAT;
            case DataType::Float: return GL_FLOAT;
        }
        return 0;
    }
}

GLTexture2D::GLTexture2D(const std::string& file, bool clamp, bool linear, bool isSRGB)
    : m_path(file), m_clamp(clamp), m_linear(linear), m_isSRGB(isSRGB)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    bind();

    auto image = Image::create(file, true);

    if (image->getData())
    {
        switch (image->getFormat())
        {
            case Image::Format::RGB:
                if (isSRGB)
                    m_internalFormat = SizedTextureFormat::sRGB8;
                else
                    m_internalFormat = SizedTextureFormat::RGB8;

                m_dataFormat = TextureFormat::RGB;
                break;
            case Image::Format::RGBA:
                if (isSRGB)
                    m_internalFormat = SizedTextureFormat::sRGBA8;
                else
                    m_internalFormat = SizedTextureFormat::RGBA8;
                m_dataFormat = TextureFormat::RGBA;
                break;
        };

        m_width = image->getWidth();
        m_height = image->getHeight();

        glTextureStorage2D(m_id, 1, Utils::getSizedTextureFormatEnumValue_(m_internalFormat), image->getWidth(), image->getHeight());

        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);

        glTextureSubImage2D(m_id,
                     0,
                     0,
                     0,
                     m_width,
                     m_height,
                     Utils::getTextureFormatEnumValue_(m_dataFormat),
                     GL_UNSIGNED_BYTE, // TODO: custom type
                     image->getData());

        glGenerateTextureMipmap(m_id);
    }
    else
    {
        Logger::getCoreLogger()->error("Image is corrupted or contains unknown formatted data!");
    }

    unbind();
}

GLTexture2D::GLTexture2D(uint32_t width, uint32_t height, SizedTextureFormat dataFormat, bool clamp, bool linear)
    : m_internalFormat(dataFormat), m_path(""), m_clamp(clamp), m_linear(linear)
    , m_isSRGB(dataFormat == SizedTextureFormat::sRGB8 || dataFormat == SizedTextureFormat::sRGBA8)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);

    bind();

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);

    glTextureStorage2D(m_id, 1, Utils::getSizedTextureFormatEnumValue_(dataFormat), width, height);
}

GLTexture2D::~GLTexture2D()
{
    glDeleteTextures(1, &m_id);
}

void GLTexture2D::setData(uint32_t xoffset, uint32_t yoffset, uint32_t width, uint32_t height, const void* data, TextureFormat dataFormat, DataType type)
{
    bind();

    glTextureSubImage2D(m_id, 0, xoffset, yoffset, width, height, Utils::getTextureFormatEnumValue_(dataFormat), Utils::getDataTypeEnumValue_(type), data);
    
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