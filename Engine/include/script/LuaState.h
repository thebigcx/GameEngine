#pragma once

#include <string>

extern "C"
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
    #include <luaconf.h>
}

//#include <sol/sol.hpp>
// TODO: Use my own lua wrapper, don't rely on sol
static int luafunc(lua_State* state)
{
    return 0;
}

class LuaState
{
public:
    LuaState(const std::string& scriptPath);
    ~LuaState();

    void script(const std::string& code)
    {
        luaL_dostring(m_state, code.c_str());
    }

    template<typename T, typename... Args>
    T callFunction(const std::string& name, Args... args)
    {
        T returnValue;

        lua_getglobal(m_state, name.c_str());

        (pushParameter<Args>(args), ...);

        lua_call(m_state, sizeof...(args), 1);

        T result = static_cast<T>(getFunctionResult<T>());

        lua_pop(m_state, 1);

        return result;
    }

    template<typename F> // function type
    void setFunction(const std::string& name, const F& func)
    {
        auto f = [](lua_State* state) -> int
        {

        };

        lua_register(m_state, name.c_str(), f);
    }

private:
    lua_State* m_state;

    template<typename T>
    void pushParameter(const T& p) {}

    template<typename T>
    T getFunctionResult() {}
};