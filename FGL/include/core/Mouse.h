#pragma once

#include "../util/maths/Vector2.h"
#include "Application.h"
#include "Logger.h"

class Mouse
{
public:
    enum Button
    {
        Left = GLFW_MOUSE_BUTTON_LEFT,
        Right = GLFW_MOUSE_BUTTON_RIGHT,
        Middle = GLFW_MOUSE_BUTTON_MIDDLE
    };

    static Vector2i getPosition();
    
    static bool isButtonPressed(Button button);

    friend class Window;

private:
    static Vector2i m_position;
};
