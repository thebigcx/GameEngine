#pragma once

#include <maths/vector/vec2.h>

namespace Engine
{

class Mouse
{
public:
    enum class Button
    {
        Left = 0,
        Right = 1,
        Middle = 2,
        Last = 8,

        One = 0,
        Two = 1,
        Three = 2,
        Four = 3,
        Five = 4,
        Six = 5,
        Seven = 6,
        Eight = 7
    };

public:
    static bool isPressed(Mouse::Button button);
    static math::uvec2 getPosition();
};

}