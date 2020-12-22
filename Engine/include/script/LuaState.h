#pragma once

#include <string>

extern "C"
{
    #include <lua/lua.h>
    #include <lua/lualib.h>
    #include <lua/lauxlib.h>
    #include <lua/luaconf.h>
}

class LuaState
{
public:
    LuaState(const std::string& scriptPath);
    ~LuaState();

    void onStart();
    void onUpdate(float dt);

    template<typename T, typename... Args>
    T callFunction(const std::string& name, Args... args)
    {
        T returnValue;

        lua_getglobal(m_state, name.c_str());

        //LuaFunctionSignature sig();
    }

private:
    lua_State* m_state;
};

template<typename... Arguments>
class LuaFunctionSignature
{

};