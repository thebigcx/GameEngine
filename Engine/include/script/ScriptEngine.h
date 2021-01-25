#pragma once

#include <core/Core.h>
#include <script/CSharpScript.h>
#include <script/mono/mono.h>
#include <events/Event.h>

namespace Engine
{
class ScriptEngine
{
public:
    ScriptEngine();
    ~ScriptEngine() = default;

    Shared<CSharpScript> loadScript(const std::string& filepath);

    void onUpdate(float dt);
    void onEvent(Event& event);
    void initialize();
    void finalize();

    // Events
    bool onMousePressed(MousePressedEvent& event);
    bool onKeyPressed(KeyPressedEvent& event);

private:
    mono::domain m_domain;
};

}