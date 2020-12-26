#include <script/ScriptEngine.h>
#include <scene/Components.h>

namespace Engine
{

ScriptEngine::ScriptEngine()
{
    m_state.open_libs();
}

void ScriptEngine::onStart()
{
    auto scripts = m_context->getRootGameObject().getChildrenWithComponent<LuaScriptComponent>();
    for (auto& object : scripts)
    {
        m_state.load_file(object->getComponent<LuaScriptComponent>().filePath);
        m_state.callVoid("onStart");
    }
}

void ScriptEngine::onUpdate(float dt)
{
    auto scripts = m_context->getRootGameObject().getChildrenWithComponent<LuaScriptComponent>();
    for (auto& object : scripts)
    {
        m_state.load_file(object->getComponent<LuaScriptComponent>().filePath);
        m_state.callVoid("onUpdate", dt);
    }
}

void ScriptEngine::onDetach()
{
    auto scripts = m_context->getRootGameObject().getChildrenWithComponent<LuaScriptComponent>();
    for (auto& object : scripts)
    {
        m_state.load_file(object->getComponent<LuaScriptComponent>().filePath);
        m_state.callVoid("onDetach");
    }
}

void ScriptEngine::onEvent(Event& event)
{
    auto scripts = m_context->getRootGameObject().getChildrenWithComponent<LuaScriptComponent>();
    for (auto& object : scripts)
    {
        m_state.load_file(object->getComponent<LuaScriptComponent>().filePath);

        EventDispatcher dispatcher(event);
        dispatcher.dispatch<MousePressedEvent>(BIND_EVENT_FN(ScriptEngine::onMousePressed));
    }
}

bool ScriptEngine::onMousePressed(MousePressedEvent& event)
{
    m_state.callVoid("onMousePressed");
    return false;
}

bool ScriptEngine::onKeyPressed(KeyPressedEvent& event)
{
    m_state.callVoid("onKeyPressed");
    return false;
}

}