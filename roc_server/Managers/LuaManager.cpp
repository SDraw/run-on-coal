#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager.h"
#include "Lua/LuaArguments.h"
#include "Lua/LuaDefinitions.Element.h"
#include "Lua/LuaDefinitions.Events.h"
#include "Lua/LuaDefinitions.File.h"
#include "Lua/LuaDefinitions.Network.h"
#include "Lua/LuaDefinitions.Utils.h"

ROC::Core* ROC::LuaManager::m_coreStatic = nullptr;

ROC::LuaManager::LuaManager(Core *f_core)
{
    m_core = f_core;
    m_coreStatic = f_core;

    m_vm = luaL_newstate();
    luaL_requiref(m_vm, "_G", luaopen_base, 1);
    luaL_requiref(m_vm, "math", luaopen_math, 1);
    luaL_requiref(m_vm, "string", luaopen_string, 1);
    luaL_requiref(m_vm, "table", luaopen_table, 1);
    luaL_requiref(m_vm, "bit32", luaopen_bit32, 1);
    luaL_requiref(m_vm, "utf8", luaopen_utf8, 1);

    //Security?
    lua_register(m_vm, "dofile", Lua::disabledFunction);
    lua_register(m_vm, "loadfile", Lua::disabledFunction);

    //Log
    lua_register(m_vm, "logPrint", Lua::logPrint);

    //File
    lua_register(m_vm, "fileCreate", Lua::fileCreate);
    lua_register(m_vm, "fileOpen", Lua::fileOpen);
    lua_register(m_vm, "fileClose", Lua::fileClose);

    lua_register(m_vm, "fileRead", Lua::fileRead);
    lua_register(m_vm, "fileWrite", Lua::fileWrite);

    lua_register(m_vm, "fileGetSize", Lua::fileGetSize);

    lua_register(m_vm, "fileSetPosition", Lua::fileSetPosition);
    lua_register(m_vm, "fileGetPosition", Lua::fileGetPosition);

    lua_register(m_vm, "fileGetPath", Lua::fileGetPath);

    lua_register(m_vm, "fileIsEOF", Lua::fileIsEOF);

    lua_register(m_vm, "fileDelete", Lua::fileDelete);
    lua_register(m_vm, "fileRename", Lua::fileRename);

    //Elements
    lua_register(m_vm, "isElement", Lua::isElement);
    lua_register(m_vm, "elementGetType", Lua::elementGetType);
    lua_register(m_vm, "elementSetData", Lua::elementSetData);
    lua_register(m_vm, "elementGetData", Lua::elementGetData);
    lua_register(m_vm, "elementRemoveData", Lua::elementRemoveData);

    //Events
    lua_register(m_vm, "addEvent", Lua::addEvent);
    lua_register(m_vm, "addEventHandler", Lua::addEventHandler);
    lua_register(m_vm, "removeEvent", Lua::removeEvent);
    lua_register(m_vm, "removeEventHandler", Lua::removeEventHandler);
    lua_register(m_vm, "callEvent", Lua::callEvent);

    //System
    lua_register(m_vm, "getTickCount", Lua::getTickCount);

    //Network
    lua_register(m_vm, "networkDisconnectClient", Lua::networkDisconnectClient);
    lua_register(m_vm, "networkSendDataToClient", Lua::networkSendDataToClient);
    lua_register(m_vm, "networkGetClientID", Lua::networkGetClientID);
    lua_register(m_vm, "networkGetClientAddress", Lua::networkGetClientAddress);
    lua_register(m_vm, "networkGetClientPing", Lua::networkGetClientPing);

    m_eventManager = new EventManager(this);
}
ROC::LuaManager::~LuaManager()
{
    lua_close(m_vm);
    delete m_eventManager;
}

bool ROC::LuaManager::OpenFile(const std::string &f_path)
{
    int error = luaL_loadfile(m_vm, f_path.c_str()) || lua_pcall(m_vm, 0, 0, 0);
    if(error)
    {
        std::string l_log(lua_tostring(m_vm, -1));
        m_core->GetLogManager()->Log(l_log);
        lua_pop(m_vm, 1);
    }
    return (error == 0);
}

void ROC::LuaManager::CallFunction(const LuaFunction &f_func, LuaArguments *f_args)
{
    lua_rawgeti(m_vm, LUA_REGISTRYINDEX, f_func.m_reference);
    f_args->ProccessArguments(m_vm);
    if(lua_pcall(m_vm, f_args->GetArgumentsCount(), 0, 0))
    {
        std::string l_log(lua_tostring(m_vm, -1));
        m_core->GetLogManager()->Log(l_log);
    }
}
