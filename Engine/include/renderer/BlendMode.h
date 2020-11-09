#pragma once

#include <GL/glew.h>

class BlendMode
{
public:
    BlendMode()
    {

    }

    BlendMode(GLenum source, GLenum destination)
        : m_source(source), m_destination(destination)
    {
        m_blending = true;

        
    }

    void bind() const
    {
        if (!m_blending)
        {
            return;
        }

        glEnable(GL_BLEND);
        glBlendFunc(m_source, m_destination);
    }

    void unbind() const
    {
        glDisable(GL_BLEND);
    }

    static BlendMode Alpha;

private:
    GLenum m_source, m_destination;

    bool m_blending = false;
};