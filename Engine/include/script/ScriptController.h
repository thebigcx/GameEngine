#pragma once

#include <vector>

#include <core/Core.h>
#include <script/CSharpScript.h>
#include <script/mono/mono.h>
#include <events/Event.h>

namespace Engine
{
class ScriptController
{
    friend class Scene;

private:
    ScriptController();

public:
    ~ScriptController();

    static ScriptController* getInstance();

    Shared<CSharpScript> loadScript(const std::string& filepath);
    void unloadScript(const Shared<CSharpScript>& script);

private:
    void initialize();
    void finalize();

public:
    bool onMousePressed(MousePressedEvent& event);
    bool onKeyPressed(KeyPressedEvent& event);
    void onEvent(Event& event);

private:
    mono::domain m_domain;

    std::vector<Shared<CSharpScript>> m_scripts;
};

}