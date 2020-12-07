#pragma once

#include <core/KeyCodes.h>
#include <maths/vector/vec2.h>

class Input
{
public:
    enum MouseButton
    {
        Left = 0,
        Right = 1,
        Middle = 2
    };

    static bool isKeyPressed(Key key);

    static math::ivec2 getMousePosition();
    
    static bool isMousePressed(MouseButton button);
};