#pragma once

#include <mono/jit/jit.h>

#include <script/mono/mono_object.h>

namespace Engine
{
namespace mono
{

class type
{
public:
    type() = default;
    type(MonoClass* t)
        : m_type(t) {}

    object create_instance();

    MonoClass* get() const { return m_type; }

private:
    MonoClass* m_type = nullptr;
};

}
}