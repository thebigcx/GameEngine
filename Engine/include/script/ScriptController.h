#pragma once

#include <vector>
#include <unordered_map>

#include <core/Core.h>
#include <script/Script.h>
#include <script/mono/Mono.h>

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

    std::unordered_map<std::string, Mono::Type> m_engineTypes;

private:
    Mono::Domain m_domain;
    Mono::Assembly m_assembly;

    std::vector<Reference<Script>> m_scripts;

    static inline MonoObject* s_currentBoxed;
};

}