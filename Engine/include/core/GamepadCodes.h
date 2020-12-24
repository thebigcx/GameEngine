#pragma once

namespace Engine
{

enum class GamepadAxis
{
    LeftX = 0,
    LeftY = 1,
    RightX = 2,
    RightY = 3,
    LeftTrigger = 4,
    RightTrigger = 5
};

enum class GamepadButton
{
    A = 0,
    B = 1,
    X = 2,
    Y = 3,
    LeftBumper = 4,
    RightBumper = 5,
    Back = 6,
    Start = 7,
    Guide = 8,
    LeftThumb = 9,
    RightThumb = 10,
    DpadUp = 11,
    DpadRight = 12,
    DpadLeft = 13,
    DpadDown = 14,
    Cross = A,
    Circle = B,
    Square = X,
    Triangle = Y
};

enum class Gamepad
{
    One = 0,
    Two = 1,
    Three = 2,
    Four = 3,
    Five = 4,
    Six = 5,
    Seven = 6,
    Eight = 7,
    Nine = 8,
    Ten = 9,
    Eleven = 10,
    Twelve = 11,
    Thirteen = 12,
    Fourteen = 13,
    Fifteen = 14,
    Sixteen = 15
};

}