#pragma once

#include <renderer/TextureCube.h>

class GLTextureCube : public TextureCube
{
public:
    GLTextureCube(const std::string& filepath);
    GLTextureCube(const std::string* files);

    void bind(uint32_t slot = 0) const override;
    void unbind(uint32_t slot = 0) const override;

private:
    static uint32_t loadFromFile(const std::string& filepath);
    static uint32_t loadFromMultipleFiles(const std::string* files);

    uint32_t m_id;
};