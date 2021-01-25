#pragma once

#include <string>

#include <mono/jit/jit.h>

namespace Engine
{
namespace mono
{

class type;

class method
{
public:
    method() = default;

private:
    method(MonoMethod* m)
        : m_method(m) {}
    method(const type& t, const std::string& name);

protected:
    MonoMethod* m_method = nullptr;

    friend class type;
};

}
}