#pragma once

#include <string>

#include <renderer/Texture2D.h>

namespace Engine
{

namespace Utils
{
    uint32_t getSizedTextureFormatEnumValue_(SizedTextureFormat format);
    uint32_t getTextureFormatEnumValue_(TextureFormat format);
}

class GLTexture2D : public Texture2D
{
public:
    GLTexture2D(const std::string& path, bool clamp = false, bool linear = true, bool isSRGB = true);
    GLTexture2D(uint32_t width, uint32_t height, SizedTextureFormat dataFormat = SizedTextureFormat::RGBA8, bool clamp = false, bool linear = true);
    ~GLTexture2D();

    void setData(uint32_t xoffset, uint32_t yoffset, uint32_t width, uint32_t height, const void* data, TextureFormat dataFormat = TextureFormat::RGBA, DataType type = DataType::UnsignedByte) override;

    void bind(uint32_t slot = 0) const override;
    void unbind(uint32_t slot = 0) const override;

    inline uint32_t getWidth() const override { return m_width; }
    inline uint32_t getHeight() const override { return m_height; }
    inline uint32_t getId() const override { return m_id; }
    inline const std::string& getPath() const override { return m_path; }

    // Returns if texture's edge is clamped
    inline bool isClamped() const override { return m_clamp; };

    // Texture filtering (nearest for hard pixels, linear for blurry pixels)
    inline bool isLinear() const override { return m_linear; };

    // Using linear color space. Should be enabled for HDR
    inline bool isSRGB() const override { return m_isSRGB; }

    bool operator==(const Texture2D& other) override;
    bool operator!=(const Texture2D& other) override;

private:
    uint32_t m_id = 0;

    SizedTextureFormat m_internalFormat;
    TextureFormat m_dataFormat;

    uint32_t m_width = 0, m_height = 0;

    bool m_clamp, m_linear, m_isSRGB;

    std::string m_path = "";

    
};

}