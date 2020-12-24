#pragma once

//#include <sol/sol.hpp>
#include <script/lua/state.h>

#include <scene/Scene.h>
#include <core/Core.h>

namespace Engine
{

// TODO: swap sol for my own lua engine
class ScriptEngine
{
public:
    ScriptEngine() = default;
    ScriptEngine(const Shared<Scene>& context)
        : m_context(context) {}

    ~ScriptEngine() = default;

    void onUpdate(float dt);
    void onEvent(Event& event);

private:
    Shared<Scene> m_context;

    lua::state m_state;
};

}