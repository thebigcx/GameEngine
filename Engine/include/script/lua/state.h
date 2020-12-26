#pragma once

#include <string>
#include <iostream>

//#include <sol/sol.hpp>

extern "C"
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
    #include <luaconf.h>
}

namespace lua
{

enum class lib
{
    base, package, math, string, io, os, table, debug, coroutine, utf8
};

// TODO: Use my own lua wrapper, don't rely on sol
static int luafunc(lua_State* state)
{
    return 0;
}

class state
{
public:
    state();
    ~state();

    void script(const std::string& code)
    {
        luaL_dostring(m_state, code.c_str());
    }

    void load_file(const std::string& path)
    {
        luaL_dofile(m_state, path.c_str());
    }

    template<typename... Args>
    void open_libs(Args&&... libs)
    {
        (open_lib(libs), ...);
        luaL_openlibs(m_state);
    }

    template<typename T, typename... Args>
    T call(const std::string& name, Args&&... args)
    {
        lua_getglobal(m_state, name.c_str());

        (push_parameter<Args>(args), ...);

        lua_call(m_state, sizeof...(args), 1);

        T result = static_cast<T>(get_function_result<T>());

        lua_pop(m_state, 1);

        return result;
    }

    template<typename... Args>
    void callVoid(const std::string& name, Args&&... args)
    {
        lua_getglobal(m_state, name.c_str());

        (push_parameter<Args>(args), ...);

        lua_call(m_state, sizeof...(args), 1);

        lua_pop(m_state, 1);
    }

    /*bool function_exists(const std::string& name)
    {
        lua_getglobal(m_state, "onMousePressed");
    }*/

    template<typename F> // function type
    void set_function(const std::string& name, const F& func)
    {
        /*lua_pushlightuserdata(m_state, &func);
        lua_pushcclosure(m_state, &luafunction, 1);
        lua_setglobal(m_state, name.c_str());*/
    }

private:
    lua_State* m_state;

    template<typename T>
    void push_parameter(const T& p);

    template<typename T>
    T get_function_result();

    template<typename T>
    void open_lib(const T& _lib)
    {
        if (_lib == lib::base)
            luaopen_base(m_state);
        else if (_lib == lib::math)
            luaopen_math(m_state);
        else if (_lib == lib::io)
            luaopen_io(m_state);
        else if (_lib == lib::package)
            luaopen_package(m_state);
        else if (_lib == lib::coroutine)
            luaopen_coroutine(m_state);
        else if (_lib == lib::math)
            luaopen_os(m_state);
        else if (_lib == lib::string)
            luaopen_string(m_state);
        else if (_lib == lib::debug)
            luaopen_debug(m_state);
        else if (_lib == lib::utf8)
            luaopen_utf8(m_state);
        else if (_lib == lib::table)
            luaopen_table(m_state);
    }
};

}