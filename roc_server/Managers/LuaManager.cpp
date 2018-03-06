#include "stdafx.h"

#include "Managers/LuaManager.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Lua/LuaArguments.h"

#include "Managers/LogManager.h"
#include "Lua/LuaDefs/LuaClientDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Lua/LuaDefs/LuaEventsDef.h"
#include "Lua/LuaDefs/LuaFileDef.h"
#include "Lua/LuaDefs/LuaUtilsDef.h"

#define ROC_LUA_METATABLE "roc_mt"

ROC::Core* ROC::LuaManager::ms_core = nullptr;

ROC::LuaManager::LuaManager(Core *f_core)
{
    m_core = f_core;

    m_vm = luaL_newstate();
    luaL_requiref(m_vm, "_G", luaopen_base, 1);
    luaL_requiref(m_vm, "math", luaopen_math, 1);
    luaL_requiref(m_vm, "string", luaopen_string, 1);
    luaL_requiref(m_vm, "table", luaopen_table, 1);
    luaL_requiref(m_vm, "bit32", luaopen_bit32, 1);
    luaL_requiref(m_vm, "utf8", luaopen_utf8, 1);
    luaL_requiref(m_vm, "package", luaopen_package, 1);

    LuaElementDef::Init(m_vm);

    LuaFileDef::Init(m_vm);
    LuaClientDef::Init(m_vm);

    LuaEventsDef::Init(m_vm);
    LuaUtilsDef::Init(m_vm);

    // Hidden metatable with weak values
    luaL_newmetatable(m_vm, ROC_LUA_METATABLE);
    lua_pushvalue(m_vm, -1);
    lua_setfield(m_vm, -2, "__index");
    lua_pushstring(m_vm, "v");
    lua_setfield(m_vm, -2, "__mode");
    lua_pushboolean(m_vm, 0);
    lua_setfield(m_vm, -2, "__metatable");
    lua_setfield(m_vm, LUA_REGISTRYINDEX, ROC_LUA_METATABLE_USERDATA);
    lua_pop(m_vm, 1);

    m_eventManager = new EventManager(this);
}
ROC::LuaManager::~LuaManager()
{
    lua_close(m_vm);
    delete m_eventManager;
}

void ROC::LuaManager::SetCore(Core *f_core)
{
    ms_core = f_core;
}

bool ROC::LuaManager::LoadScript(const std::string &f_script, bool f_asFile)
{
    int l_error = ((f_asFile ? luaL_loadfile(m_vm, f_script.c_str()) : luaL_loadstring(m_vm, f_script.c_str())) || lua_pcall(m_vm, 0, 0, 0));
    if(l_error)
    {
        std::string l_log(lua_tostring(m_vm, -1));
        m_core->GetLogManager()->Log(l_log);
        lua_pop(m_vm, 1);
    }
    return (l_error == 0);
}

void ROC::LuaManager::CallFunction(const LuaFunction &f_func, const LuaArguments *f_args)
{
    lua_rawgeti(m_vm, LUA_REGISTRYINDEX, f_func.m_ref);

    for(auto &iter : f_args->GetArgumentsVectorRef())
    {
        switch(iter.GetType())
        {
            case CustomData::CDT_Nil:
                lua_pushnil(m_vm);
                break;
            case CustomData::CDT_Boolean:
                lua_pushboolean(m_vm, iter.GetBoolean());
                break;
            case CustomData::CDT_Integer:
                lua_pushinteger(m_vm, iter.GetInteger());
                break;
            case CustomData::CDT_Double:
                lua_pushnumber(m_vm, iter.GetDouble());
                break;
            case CustomData::CDT_Float:
                lua_pushnumber(m_vm, iter.GetFloat());
                break;
            case CustomData::CDT_Element:
            {
                void *l_ptr;
                std::string l_className;
                iter.GetElement(l_ptr, l_className);

                luaL_getmetatable(m_vm, ROC_LUA_METATABLE_USERDATA);
                lua_pushlightuserdata(m_vm, l_ptr);
                lua_rawget(m_vm, -2);
                if(lua_isnil(m_vm, -1))
                {
                    lua_pop(m_vm, 1);
                    *reinterpret_cast<void**>(lua_newuserdata(m_vm, sizeof(void*))) = l_ptr;
                    luaL_setmetatable(m_vm, l_className.c_str());
                    lua_pushlightuserdata(m_vm, l_ptr);
                    lua_pushvalue(m_vm, -2);
                    lua_rawset(m_vm, -4);
                }
                lua_remove(m_vm, -2);
            } break;
            case CustomData::CDT_String:
            {
                const std::string &l_string = iter.GetString();
                lua_pushlstring(m_vm, l_string.data(), l_string.size());
            } break;
        }
    }
    if(lua_pcall(m_vm, f_args->GetArgumentsCount(), 0, 0))
    {
        std::string l_log(lua_tostring(m_vm, -1));
        m_core->GetLogManager()->Log(l_log);
        lua_pop(m_vm, 1);
    }
}
