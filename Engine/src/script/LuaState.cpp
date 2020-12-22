#include <script/LuaState.h>

LuaState::LuaState(const std::string& scriptPath)
{
    m_state = luaL_newstate();

    luaL_openlibs(m_state);

    luaL_dofile(m_state, scriptPath.c_str());
}

LuaState::~LuaState()
{
    lua_close(m_state);
}