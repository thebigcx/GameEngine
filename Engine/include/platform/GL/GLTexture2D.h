#pragma once

#include <string>

#include <renderer/Texture2D.h>

class GLTexture2D : public Texture2D
{
public:
    GLTexture2D(const std::string& path);
    GLTexture2D(uint32_t width, uint32_t height, GLenum dataFormat = GL_RGBA8);
    ~GLTexture2D();

    void setData(float xoffset, float yoffset, float width, float height, const void* data, GLenum dataFormat = GL_RGBA) override;
    void setParameter(Parameter parameter, Value value) override;

    void bind(int slot = 0) const override;
    void unbind(int slot = 0) const override;

    float getWidth() const override;
    float getHeight() const override;

    unsigned int getId() const override;

private:
    unsigned int m_id = 0;

    bool m_mipmapped = false;
    GLenum m_internalFormat;
    GLenum m_dataFormat;

    float m_width, m_height;
};