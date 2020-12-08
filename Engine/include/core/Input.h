#pragma once

#include <core/KeyCodes.h>
#include <core/MouseCodes.h>
#include <core/GamepadCodes.h>
#include <maths/vector/vec2.h>

class Input
{
public:
    static bool isKeyPressed(Key key);
    static bool isMousePressed(MouseButton button);

    static math::ivec2 getMousePosition();

    static bool isGamepadButtonPressed(Gamepad gamepad, GamepadButton button);
    static float getGamepadAxis(Gamepad gamepad, GamepadAxis axis);
    static void setGamepadMappingsFromFile(const std::string& path);
};