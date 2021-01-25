#include <script/mono/mono_type.h>
#include <script/mono/mono_domain.h>
#include <script/mono/mono_object.h>

namespace Engine
{
namespace mono
{

type::type(MonoClass* c)
    : m_class(c)
{
    generate_meta();
}

object type::create_instance()
{
    object obj(mono_object_new(domain::get_current_domain()->get(), m_class));
    mono_runtime_object_init(obj.get());
    return obj;
}

method type::get_method(const std::string& name)
{
    return method(*this, name);
}

void type::generate_meta()
{
    m_type = mono_class_get_type(m_class);
    m_namespace = mono_class_get_namespace(m_class);
    m_name = mono_class_get_name(m_class);
    m_fullname = m_namespace.empty() ? m_name : m_namespace + "." + m_name;
}

}
}