#include <script/CSharpScript.h>

namespace Engine
{

Reference<CSharpScript> CSharpScript::create(const std::string& filepath, const Mono::Domain& domain)
{
    CSharpScript* script = new CSharpScript(filepath, domain);
    return std::shared_ptr<CSharpScript>(script);
}

CSharpScript::CSharpScript(const std::string& filepath, const Mono::Domain& domain)
{
    m_assembly = domain.getAssembly(filepath);
}

void CSharpScript::onStart()
{

}

void CSharpScript::onUpdate(float dt)
{

}

void CSharpScript::onDestroy()
{

}

}