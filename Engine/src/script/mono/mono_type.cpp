#include <script/mono/mono_type.h>
#include <script/mono/mono_domain.h>
#include <script/mono/mono_object.h>

namespace Engine
{
namespace mono
{

object type::create_instance()
{
    object obj(mono_object_new(domain::get_current_domain()->get(), m_type));
    mono_runtime_object_init(obj.get());
    return obj;
}

}
}