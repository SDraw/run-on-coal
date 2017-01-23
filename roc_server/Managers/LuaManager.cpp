#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager.h"
#include "Lua/LuaArguments.h"
#include "Lua/LuaDefinitions.Events.h"
#include "Lua/LuaDefinitions.Network.h"
#include "Lua/LuaDefinitions.Sfml.h"
#include "Lua/LuaDefinitions.Utils.h"

ROC::Core* ROC::LuaManager::m_corePointer = NULL;

ROC::LuaManager::LuaManager(Core *f_core)
{
    m_core = f_core;
    m_corePointer = f_core;

    m_pVM = luaL_newstate();
    luaL_requiref(m_pVM, "_G", luaopen_base, 1);
    luaL_requiref(m_pVM, "math", luaopen_math, 1);
    luaL_requiref(m_pVM, "string", luaopen_string, 1);
    luaL_requiref(m_pVM, "table", luaopen_table, 1);
    luaL_requiref(m_pVM, "bit32", luaopen_bit32, 1);
    luaL_requiref(m_pVM, "utf8", luaopen_utf8, 1);

    //Security?
    lua_register(m_pVM, "dofile", Lua::disabledFunction);
    lua_register(m_pVM, "loadfile", Lua::disabledFunction);

    //Log
    lua_register(m_pVM, "logPrint", Lua::logPrint);

    //Sfml
    lua_register(m_pVM, "getTime", Lua::getTime);

    //Elements
    lua_register(m_pVM, "isElement", Lua::isElement);
    lua_register(m_pVM, "getElementType", Lua::getElementType);

    //Events
    lua_register(m_pVM, "addEvent", Lua::addEvent);
    lua_register(m_pVM, "setEventMute", Lua::setEventMute);
    lua_register(m_pVM, "removeEvent", Lua::removeEvent);

    //System
    lua_register(m_pVM, "getTickCount", Lua::getTickCount);

    //Network
    lua_register(m_pVM, "networkDisconnectClient", Lua::networkDisconnectClient);
    lua_register(m_pVM, "networkSendDataToClient", Lua::networkSendDataToClient);
    lua_register(m_pVM, "networkGetClientID", Lua::networkGetClientID);
    lua_register(m_pVM, "networkGetClientAddress", Lua::networkGetClientAddress);
    lua_register(m_pVM, "networkGetClientPing", Lua::networkGetClientPing);

    m_eventManager = new EventManager(this);
}
ROC::LuaManager::~LuaManager()
{
    lua_close(m_pVM);
    delete m_eventManager;
}

bool ROC::LuaManager::OpenFile(std::string &f_path)
{
    int error = luaL_loadfile(m_pVM, f_path.c_str()) || lua_pcall(m_pVM, 0, 0, 0);
    if(error)
    {
        std::string l_log;
        l_log.append(lua_tostring(m_pVM, -1));
        m_core->GetLogManager()->Log(l_log);
        lua_pop(m_pVM, 1);
    }
    return (error == 0);
}

void ROC::LuaManager::CallFunction(int f_func, LuaArguments *f_args)
{
    lua_rawgeti(m_pVM, LUA_REGISTRYINDEX, f_func);
    f_args->ProccessArguments(m_pVM);
    if(lua_pcall(m_pVM, f_args->GetArgumentsValue(), 0, 0))
    {
        std::string l_log;
        l_log.append(lua_tostring(m_pVM, -1));
        m_core->GetLogManager()->Log(l_log);
    }
}
