#pragma once

#include <mono/jit/jit.h>

#include <script/mono/mono_method.h>

namespace Engine
{
namespace mono
{

class object;

class type
{
public:
    type() = default;
    type(MonoClass* t);

    object create_instance();

    method get_method(const std::string& name);

    MonoClass* get() const { return m_class; }

    const std::string& get_name() const { return m_name; }
    const std::string& get_fullname() const { return m_fullname; }
    const std::string& get_namespace() const { return m_namespace; }

private:
    MonoClass* m_class = nullptr;
    MonoType* m_type = nullptr;

    void generate_meta();

    std::string m_name;
    std::string m_fullname;
    std::string m_namespace;
};

}
}