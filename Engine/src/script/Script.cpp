#include <script/Script.h>

namespace Engine
{

Reference<Script> Script::create(const std::string& filepath, const Mono::Domain& domain)
{
    Script* script = new Script(filepath, domain);
    return std::shared_ptr<Script>(script);
}

Script::Script(const std::string& filepath, const Mono::Domain& domain)
    : m_path(filepath)
{
    
}

void Script::onStart()
{

}

void Script::onUpdate(float dt)
{

}

void Script::onDestroy()
{

}

}