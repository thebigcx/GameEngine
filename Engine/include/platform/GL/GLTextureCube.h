#pragma once

#include <renderer/TextureCube.h>

namespace Engine
{

class GLTextureCube : public TextureCube
{
public:
    GLTextureCube(const std::string& filepath);
    GLTextureCube(const std::string* files);

    void bind(uint32_t slot = 0) const override;
    void unbind(uint32_t slot = 0) const override;

    inline bool operator==(const TextureCube& cube) const override
    {
        return static_cast<const GLTextureCube&>(cube).m_id == m_id;
    }

    inline bool operator!=(const TextureCube& cube) const override
    {
        return static_cast<const GLTextureCube&>(cube).m_id != m_id;
    }

private:
    static uint32_t loadFromFile(const std::string& filepath);
    static uint32_t loadFromMultipleFiles(const std::string* files);

    uint32_t m_id = 0;
};

}