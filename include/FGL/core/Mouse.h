#pragma once

#include <GLFW/glfw3.h>

#include "../util/maths/Vector2.h"
#include "Application.h"
#include "Logger.h"

class Mouse
{
public:
    static Vector2i getPosition()
    {
        auto window = Application::get().getActiveWindow().getId();
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return Vector2i(x, y);
    }
    
    static bool isMousePressed(int button);
};