#pragma once

#include <iostream>
#include <ostream>
#include <cstdarg>

class Logger
{
public:
    template <typename T>
    static void out(const T& log)
    {
        std::cout << log << "\n";
    }

    static void err(const std::string& what);

    static void init();

    static std::ostream& cout()
    {
        return std::cout;
    }
};