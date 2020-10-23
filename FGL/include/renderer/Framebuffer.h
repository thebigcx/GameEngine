#pragma once

#include <array>

#include <renderer/Shader.h>
#include <renderer/Buffer.h>

class Framebuffer
{
public:
    Framebuffer();
    ~Framebuffer();

    void create();

    void render(Shader& shader);

    void bind() const;
    void unbind() const;

private:
    unsigned int m_id;
    unsigned int m_textureId;
    unsigned int m_renderBuf;

    unsigned int m_vao;
    unsigned int m_vertexBuffer;

    static inline const float m_vertices[] = {
        -1,  1, 0, 1,
        -1, -1, 0, 0,
         1, -1, 1, 0,
        -1,  1, 0, 1,
         1, -1, 1, 0,
         1,  1, 1, 1
    };
};