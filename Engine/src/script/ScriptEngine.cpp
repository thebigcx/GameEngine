#include <script/ScriptEngine.h>
#include <scene/Components.h>
#include <scene/Scene.h>

namespace Engine
{

ScriptEngine::ScriptEngine()
{
    
}

Shared<CSharpScript> ScriptEngine::loadScript(const std::string& filepath)
{
    auto script = CSharpScript::create(filepath, m_domain);

    return script;
}

void ScriptEngine::initialize()
{
    mono::init("mono");
    m_domain.create("Engine");
}

void ScriptEngine::onUpdate(float dt)
{
    
}

void ScriptEngine::finalize()
{
    mono::shutdown();
}

void ScriptEngine::onEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MousePressedEvent>(BIND_EVENT_FN(ScriptEngine::onMousePressed));
}

bool ScriptEngine::onMousePressed(MousePressedEvent& event)
{
    
    return false;
}

bool ScriptEngine::onKeyPressed(KeyPressedEvent& event)
{
    
    return false;
}

}