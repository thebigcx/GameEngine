#pragma once

#include <GL/glew.h>
#include <stb_image/stb_image.h>

#include <string>

#include "../util/maths/Vector2.h"

class Texture {
public:
    Texture();

    ~Texture();

    void bind() const;

    bool loadFile(const std::string& file);

    void setSmooth(bool smooth = true);
    bool isSmooth() const;

    void setRepeated(bool repeated = true);
    bool isRepeated() const;

    void setVerticalFlip(bool flip = true);

    Vector2u getSize() const;

private:
    unsigned int m_id;

    bool m_smooth = false;
    bool m_repeated = false;
    bool m_mipmapped = false;
    bool m_flipped = true;

    Vector2u m_size;
};