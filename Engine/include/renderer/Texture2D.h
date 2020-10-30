#pragma once

#include <GL/glew.h>
#include <stb_image/stb_image.h>

#include <string>

#include <math/vector/Vector2.h>
#include <util/Image.h>
#include <core/Core.h>

class Texture2D {
public:
    Texture2D() {}

    ~Texture2D();

    Texture2D(Texture2D&& texture);

    static Shared<Texture2D> create(const std::string& file);
    static Shared<Texture2D> create(int width, int height);

    void bind() const;

    void setSmooth(bool smooth = true);
    bool isSmooth() const;

    void setRepeated(bool repeated = true);
    bool isRepeated() const;

    Vector2f getSize() const;

    inline int getId() const { return m_id; }

private:
    unsigned int m_id;

    bool m_smooth = false;
    bool m_repeated = false;
    bool m_mipmapped = false;
    GLenum m_internalFormat;
    GLenum m_dataFormat;

    Vector2f m_size;
};