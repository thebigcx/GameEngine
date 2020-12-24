#include <script/lua/state.h>

#include <iostream>

namespace lua
{

state::state()
{
    m_state = luaL_newstate();
}

state::~state()
{
    lua_close(m_state);
}

template<>
void state::push_parameter<int>(const int& p)
{
    lua_pushnumber(m_state, p);
}

template<>
void state::push_parameter<std::string>(const std::string& p)
{
    lua_pushstring(m_state, p.c_str());
}

template<>
void state::push_parameter<bool>(const bool& p)
{
    lua_pushboolean(m_state, p);
}

template<>
int state::get_function_result<int>()
{
    return lua_tointeger(m_state, -1);
}

template<>
std::string state::get_function_result<std::string>()
{
    return std::string(lua_tostring(m_state, -1));
}

template<>
bool state::get_function_result<bool>()
{
    return static_cast<bool>(lua_toboolean(m_state, -1));
}

}