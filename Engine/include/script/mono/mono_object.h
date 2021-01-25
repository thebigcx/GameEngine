#pragma once

#include <mono/jit/jit.h>

#include <script/mono/mono_type.h>

namespace Engine
{
namespace mono
{

class object
{
public:
    object() = default;
    object(MonoObject* obj)
        : m_object(obj), m_type(type(mono_object_get_class(obj))) {}

    const type& get_type() const { return m_type; }

    MonoObject* get() const { return m_object; }

private:
    MonoObject* m_object = nullptr;
    type m_type = nullptr;
};

}
}