#pragma once

#include <iostream>

class Logger
{
public:
    template <typename T>
    static void log(const T& log)
    {
        std::cout << log << "\n";
    }

    static void init();
};