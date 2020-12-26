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
    m_state.callVoid("onCreate");
}

void ScriptEngine::onUpdate(float dt)
{
    m_state.callVoid("onUpdate", dt);
}

void ScriptEngine::onDetach()
{
    m_state.callVoid("onDestroy");
}

void ScriptEngine::onEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MousePressedEvent>(BIND_EVENT_FN(ScriptEngine::onMousePressed));
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