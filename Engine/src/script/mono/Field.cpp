#include <script/mono/Field.h>
#include <script/mono/Exception.h>
#include <script/mono/Domain.h>

#include <mono/metadata/attrdefs.h>
#include <mono/metadata/debug-helpers.h>

#include <cassert>

namespace Engine
{
namespace Mono
{

Field::Field(const Type& type, const std::string& name)
    : m_field(mono_class_get_field_from_name(type.get(), name.c_str())), m_name(name)
{
    if (!m_field)
        throw Exception("Could not get field: " + name + " in class " + type.getName());

    auto domain = Domain::getCurrentDomain();

    generateMeta();

    if (isStatic())
    {
        m_typeVtable = mono_class_vtable(domain->get(), type.get());
        mono_runtime_class_init(m_typeVtable);
    }
}

bool Field::isValueType() const
{
    return m_type.isValueType();
}

Access Field::getAccessLevel() const
{
    uint32_t access = mono_field_get_flags(m_field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;

    switch (access)
    {
        case MONO_FIELD_ATTR_PRIVATE:
            return Access::Private;
        case MONO_FIELD_ATTR_FAM_AND_ASSEM:
            return Access::ProtectedInternal;
        case MONO_FIELD_ATTR_ASSEMBLY:
            return Access::Internal;
        case MONO_FIELD_ATTR_FAMILY:
            return Access::Protected;
        case MONO_FIELD_ATTR_PUBLIC:
            return Access::Public;
    };

    assert(false);

    return Access::Private;
}

bool Field::isStatic() const
{
    uint32_t flags = mono_field_get_flags(m_field);
    return (flags & MONO_FIELD_ATTR_STATIC) != 0;
}

void Field::generateMeta()
{
    auto type = mono_field_get_type(m_field);
    m_type = Type(mono_class_from_mono_type(type));
    m_fullname = mono_field_full_name(m_field);
    std::string storage = (isStatic() ? " static " : " ");
    m_fullDeclname = toString(getAccessLevel()) + storage = m_fullname;
}

}
}