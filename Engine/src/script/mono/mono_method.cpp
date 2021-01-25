#include <script/mono/mono_method.h>
#include <script/mono/mono_type.h>

#include <mono/metadata/debug-helpers.h>

namespace Engine
{
namespace mono
{

method::method(const type& t, const std::string& name)
{
    auto method_desc = mono_method_desc_new((":" + name).c_str(), 0);
    m_method = mono_method_desc_search_in_class(method_desc, t.get());
    mono_method_desc_free(method_desc);
}

}
}