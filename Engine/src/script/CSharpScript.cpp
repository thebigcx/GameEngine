#include <script/CSharpScript.h>

namespace Engine
{

Shared<CSharpScript> CSharpScript::create(const std::string& filepath, const mono::domain& domain)
{
    CSharpScript* script = new CSharpScript(filepath, domain);
    return std::shared_ptr<CSharpScript>(script);
}

CSharpScript::CSharpScript(const std::string& filepath, const mono::domain& domain)
{
    m_assembly = domain.get_assembly(filepath);
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