#pragma once

#include <mono/jit/jit.h>

namespace Engine
{
namespace mono
{

class object
{
public:
    object() = default;
    object(MonoObject* obj)
        : m_object(obj) {}

    MonoObject* get() const { return m_object; }

private:
    MonoObject* m_object = nullptr;
};

}
}