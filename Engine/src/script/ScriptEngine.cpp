#include <script/ScriptEngine.h>
#include <scene/Components.h>

namespace Engine
{

void ScriptEngine::onUpdate(float dt)
{
    auto view = m_context->getRegistry().recurse_view<LuaScriptComponent>();
    for (auto& entityID : view)
    {
        
    }
}

}