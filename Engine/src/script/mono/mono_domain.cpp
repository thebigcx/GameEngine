#include <script/mono/mono_domain.h>
#include <core/Logger.h>

#include <mono/metadata/mono-config.h>

namespace Engine
{
namespace mono
{

static MonoDomain* s_domain = nullptr;

bool init(const std::string& dom)
{
    mono_config_parse("/etc/mono/config");
    mono_set_dirs("/usr/lib", "/etc");

    s_domain = mono_jit_init(dom.c_str());

    return static_cast<bool>(s_domain);
}

void shutdown()
{
    mono_jit_cleanup(s_domain);
}

domain::domain(const std::string& name)
{
    m_domain = mono_domain_create_appdomain(const_cast<char*>(name.c_str()), nullptr);
}

void domain::create(const std::string& name)
{
    m_domain = mono_domain_create_appdomain(const_cast<char*>(name.c_str()), nullptr);
}

assembly domain::get_assembly(const std::string& file)
{
    assembly assem(mono_domain_assembly_open(m_domain, file.c_str()));
    
    if (!assem.get())
    {
        Logger::getCoreLogger()->error("[MONO] Could not open assembly: %s", file.c_str());
    }
    
    return assem;
}

void domain::set_current_domain(const domain& dom)
{
    s_current_domain = &dom;
}

}
}