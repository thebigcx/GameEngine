#include <script/ScriptController.h>
#include <scene/Components.h>
#include <scene/Scene.h>
#include <util/io/FileSystem.h>

#include <events/Event.h>
#include <events/EventDispatcher.h>
#include <events/MouseEvent.h>
#include <events/KeyboardEvent.h>

#include <Mono/Compiler.h>

namespace Engine
{

ScriptController::ScriptController()
{
    
}

ScriptController::~ScriptController()
{

}

ScriptController* ScriptController::getInstance()
{
    static ScriptController controller;
    return &controller;
}

void ScriptController::recompileScripts()
{
    Mono::Compiler compiler("/usr/bin/mcs");
    compiler.buildLibrary("Game.dll", "Editor/scripts/Engine.cs");

    m_assembly = Mono::Assembly(Mono::getCurrentDomain(), "Game.dll");
}

Reference<Script> ScriptController::loadScript(const std::string& filepath)
{
    auto script = Script::create(filepath, Mono::getCurrentDomain());
    m_scripts.push_back(script);

    recompileScripts();

    return script;
}

void ScriptController::unloadScript(const Reference<Script>& script)
{
    for (std::vector<Reference<Script>>::iterator iter = m_scripts.begin(); iter != m_scripts.end(); iter++)
    {
        if (*iter == script)
        {
            m_scripts.erase(iter);
            break;
        }
    }
    
    recompileScripts();
}

void ScriptController::initialize()
{
    Mono::init("/usr/lib", "/etc", "Engine");
}

void ScriptController::finalize()
{
    Mono::shutdown();
}

void ScriptController::onEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MousePressedEvent>(BIND_EVENT_FN(ScriptController::onMousePressed));
}

bool ScriptController::onMousePressed(MousePressedEvent& event)
{
    
    return false;
}

bool ScriptController::onKeyPressed(KeyPressedEvent& event)
{
    
    return false;
}

}