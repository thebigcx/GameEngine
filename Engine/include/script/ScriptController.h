#pragma once

#include <vector>

#include <core/Core.h>
#include <script/CSharpScript.h>
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

    Reference<CSharpScript> loadScript(const std::string& filepath);
    void unloadScript(const Reference<CSharpScript>& script);

private:
    void initialize();
    void finalize();

public:
    bool onMousePressed(MousePressedEvent& event);
    bool onKeyPressed(KeyPressedEvent& event);
    void onEvent(Event& event);

private:
    Mono::Domain m_domain;

    std::vector<Reference<CSharpScript>> m_scripts;
};

}