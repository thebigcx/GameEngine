#pragma once

#include <iostream>
#include <ostream>
#include <cstdarg>

class Console
{
public:
    template <typename T>
    static void out(const T& log)
    {
        std::cout << log << "\n";
    }

    template <typename ...Args>
    static void outf(const std::string& format, Args... args)
    {
        char tmp[1024];
        snprintf(tmp, 1024, format.c_str(), args...);
        std::cout << tmp << "\n";
    }

    template <typename T, typename ...Args>
    static void out(const T& t, Args... args)
    {
        std::cout << t << "\n";

        out(args...);
    }

    static void err(const std::string& what)
    {
        std::cout << "* Runtime error *\n";
        std::cout << what << "\n\n";

        throw std::runtime_error("Error thrown by Console::err()");
    }

    template <typename ...Args>
    static void errf(const std::string& what, Args... args)
    {
        char tmp[1024];
        snprintf(tmp, 1024, what.c_str(), args...);
        throw std::runtime_error(tmp);
    }

    static void init();

    static std::ostream& cout()
    {
        return std::cout;
    }
};