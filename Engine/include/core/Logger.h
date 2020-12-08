#pragma once

#include <iostream>
#include <chrono>

#include <core/Core.h>

class Logger
{
public:
    template<typename ...Args>
    void warn(const std::string& str, Args... args)
    {
        std::cout << "[WARNING] " << getFormat(str, args...) << "\n";
    }

    void warn(const std::string& str)
    {
        std::cout << "[WARNING] " << str << "\n";
    }

    template<typename ...Args>
    void error(const std::string& str, Args... args)
    {
        std::cout << "[ERROR] " << getFormat(str, args...) << "\n";
    }

    void error(const std::string& str)
    {
        std::cout << "[ERROR] " << str << "\n";
    }

    template<typename ...Args>
    void info(const std::string& str, Args... args)
    {
        std::cout << "[INFO] " << getFormat(str, args...) << "\n";
    }

    void info(const std::string& str)
    {
        std::cout << "[INFO] " << str << "\n";
    }

    template<typename ...Args>
    void critical(const std::string& str, Args... args)
    {
        std::cout << "[CRITICAL] " << getFormat(str, args...) << "\n";
    }

    void critical(const std::string& str)
    {
        std::cout << "[CRITICAL] " << str << "\n";
    }

    template<typename... Args>
    void trace()
    {
        
    }

    template<typename... Args>
    std::string format(const std::string& str, Args... args)
    {
        return getFormat(str, args...);
    }

    static Shared<Logger> getCoreLogger()
    {
        return m_coreLogger;
    }

private:
    static Shared<Logger> m_coreLogger;

    template<typename ...Args>
    std::string getFormat(const std::string& format, Args... args)
    {
        char tmp[1024];
        snprintf(tmp, 1024, format.c_str(), args...);
        return tmp;
    }
};