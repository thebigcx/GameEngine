#include <script/ScriptEngine.h>
#include <scene/Components.h>

namespace Engine
{

void ScriptEngine::onUpdate(float dt)
{
    auto scripts = m_context->getRootGameObject().getChildrenWithComponent<LuaScriptComponent>();
    for (auto& object : scripts)
    {
        
    }
}

}