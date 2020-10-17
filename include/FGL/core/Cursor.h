#pragma once

#include <GLFW/glfw3.h>

#include "Application.h"

class Cursor
{
public:
    enum NativeCursor
    {
        Arrow = GLFW_ARROW_CURSOR,
        IBeam = GLFW_IBEAM_CURSOR,
        Crosshair = GLFW_CROSSHAIR_CURSOR,
        Hand = GLFW_HAND_CURSOR,
        HorizontalResize = GLFW_HRESIZE_CURSOR,
        VerticalResize = GLFW_VRESIZE_CURSOR
    };

    Cursor()
    {

    }

    ~Cursor()
    {
        glfwDestroyCursor(m_cursor);
    }

    void setCurrent(bool current = true)
    {
        auto& window = Application::get().getWindow();
        if (current)
        {
            glfwSetCursor(window.getNative(), m_cursor);
        }
        else
        {
            glfwSetCursor(window.getNative(), nullptr);
        }
    }

    void createNativeCursor(NativeCursor cursor)
    {
        m_cursor = glfwCreateStandardCursor(cursor);
    }

    

private:
    GLFWcursor* m_cursor;
};