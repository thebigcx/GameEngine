#pragma once

#include <vector>
#include <unordered_map>

#include <core/Core.h>
#include <script/Script.h>
#include <Mono/Type.h>

namespace Engine
{

class Event;
class KeyPressedEvent;
class MousePressedEvent;

class ScriptController
{
    friend class Game;

private:
    ScriptController();

public:
    ~ScriptController();

    static ScriptController* getInstance();

    Reference<Script> loadScript(const std::string& filepath);
    void unloadScript(const Reference<Script>& script);

private:
    void initialize();
    void finalize();

    void recompileScripts();

public:
    bool onMousePressed(MousePressedEvent& event);
    bool onKeyPressed(KeyPressedEvent& event);
    void onEvent(Event& event);

private:
    Mono::Assembly m_assembly;

    std::vector<Reference<Script>> m_scripts;
};

}