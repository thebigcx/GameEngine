#include <script/LuaState.h>

#include <iostream>

LuaState::LuaState(const std::string& scriptPath)
{
    m_state = luaL_newstate();

    luaL_openlibs(m_state);
}

LuaState::~LuaState()
{
    lua_close(m_state);
}

template<>
void LuaState::pushParameter<int>(const int& p)
{
    lua_pushnumber(m_state, p);
}

template<>
void LuaState::pushParameter<std::string>(const std::string& p)
{
    lua_pushstring(m_state, p.c_str());
}

template<>
void LuaState::pushParameter<bool>(const bool& p)
{
    lua_pushboolean(m_state, p);
}

template<>
int LuaState::getFunctionResult<int>()
{
    return lua_tointeger(m_state, -1);
}

template<>
std::string LuaState::getFunctionResult<std::string>()
{
    return std::string(lua_tostring(m_state, -1));
}

template<>
bool LuaState::getFunctionResult<bool>()
{
    return static_cast<bool>(lua_toboolean(m_state, -1));
}