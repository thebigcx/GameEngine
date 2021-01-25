#pragma once

#include <string>
#include <stdexcept>

#include <mono/jit/jit.h>

namespace Engine
{
namespace mono
{

class exception : public std::runtime_error
{
public:
    exception(const std::string& what)
        : std::runtime_error(what) {}
};

class thunk_exception : public exception
{
public:
    thunk_exception(MonoObject* obj);

    struct exception_info
    {
        std::string exception_typename;
        std::string message;
        std::string stacktrace;
    };

private:
    thunk_exception(const exception_info& inf);

    exception_info m_info;
};

}
}