#pragma once

#include <string>

#include <renderer/Texture2D.h>

namespace Engine
{

class GLTexture2D : public Texture2D
{
public:
    GLTexture2D(const std::string& path, bool isSRGB = true);
    GLTexture2D(uint32_t width, uint32_t height, GLenum dataFormat = GL_RGBA8);
    ~GLTexture2D();

    void setData(float xoffset, float yoffset, float width, float height, const void* data, GLenum dataFormat = GL_RGBA) override;
    void setParameter(Parameter parameter, Value value) override;

    void bind(uint32_t slot = 0) const override;
    void unbind(uint32_t slot = 0) const override;

    inline float getWidth() const override { return m_width; }
    inline float getHeight() const override { return m_height; }
    inline uint32_t getId() const override { return m_id; }
    inline const std::string& getPath() const { return m_path; }

    bool operator==(const Texture2D& other) override;
    bool operator!=(const Texture2D& other) override;

private:
    uint32_t m_id = 0;

    bool m_mipmapped = false;
    GLenum m_internalFormat;
    GLenum m_dataFormat;

    float m_width = 0.f, m_height = 0.f;

    std::string m_path = "";
};

}