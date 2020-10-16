#pragma once

#include <GLFW/glfw3.h>

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

    static Vector2i getPosition()
    {
        auto window = Application::get().getActiveWindow().getId();
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return Vector2i(x, y);
    }
    
    static bool isButtonPressed(Button button)
    {
        auto window = Application::get().getActiveWindow().getId();

        return glfwGetMouseButton(window, button) == GLFW_PRESS;
    }
};