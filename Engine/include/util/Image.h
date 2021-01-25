#pragma once

#include <string>

#include <core/Core.h>

namespace Engine
{
    
class Image
{
public:
    enum class Format
    {
        RGB, RGBA
    };

    ~Image();

public:
    static Shared<Image> create(const std::string& path, bool flipped = false);

    inline unsigned int getWidth() const { return m_width; }
    inline unsigned int getHeight() const { return m_height; }
    inline Image::Format getFormat() const { return m_format; }
    inline void* getData() const { return m_data; }

private:
    Image();
   

    unsigned int m_width;
    unsigned int m_height;
    Format m_format;
    void* m_data;
};

}