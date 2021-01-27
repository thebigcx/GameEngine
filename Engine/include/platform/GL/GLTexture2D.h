#pragma once

#include <string>

#include <renderer/Texture2D.h>

namespace Engine
{

namespace Utils
{
    GLenum getSizedTextureFormatEnumValue_(SizedTextureFormat format);
    GLenum getTextureFormatEnumValue_(TextureFormat format);
}

class GLTexture2D : public Texture2D
{
public:
    GLTexture2D(const std::string& path, bool clamp = false, bool linear = true);
    GLTexture2D(uint32_t width, uint32_t height, SizedTextureFormat dataFormat = SizedTextureFormat::RGBA8, bool clamp = false, bool linear = true);
    ~GLTexture2D();

    void setData(uint32_t xoffset, uint32_t yoffset, uint32_t width, uint32_t height, const void* data, TextureFormat dataFormat = TextureFormat::RGBA, DataType type = DataType::UnsignedByte) override;

    void bind(uint32_t slot = 0) const override;
    void unbind(uint32_t slot = 0) const override;

    inline uint32_t getWidth() const override { return m_width; }
    inline uint32_t getHeight() const override { return m_height; }
    inline uint32_t getId() const override { return m_id; }
    inline const std::string& getPath() const override { return m_path; }

    bool operator==(const Texture2D& other) override;
    bool operator!=(const Texture2D& other) override;

private:
    uint32_t m_id = 0;

    SizedTextureFormat m_internalFormat;
    TextureFormat m_dataFormat;

    uint32_t m_width = 0, m_height = 0;

    std::string m_path = "";

    
};

}