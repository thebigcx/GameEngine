#pragma once

//#include <sol/sol.hpp>
#include <script/lua/state.h>

#include <scene/Scene.h>
#include <core/Core.h>

namespace Engine
{
// TODO: look into replacing lua with C#
class ScriptEngine
{
public:
    ScriptEngine();

    ~ScriptEngine() = default;

    void setScript(const std::string& path)
    {
        m_filePath = path;
        m_state.load_file(m_filePath);
    }

    void onUpdate(float dt);
    void onEvent(Event& event);
    void onStart();
    void onDetach();

    // Events
    bool onMousePressed(MousePressedEvent& event);
    bool onKeyPressed(KeyPressedEvent& event);

private:
    lua::state m_state;

    std::string m_filePath;
};

}