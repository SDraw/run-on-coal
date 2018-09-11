#include "stdafx.h"

#include "Managers/LuaManager.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Lua/LuaArguments.h"
#include "Lua/LuaFunction.h"

#include "Managers/LogManager.h"
#include "Elements/Element.h"
#include "Lua/LuaDefs/LuaAnimationDef.h"
#include "Lua/LuaDefs/LuaCameraDef.h"
#include "Lua/LuaDefs/LuaCollisionDef.h"
#include "Lua/LuaDefs/LuaDrawableDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Lua/LuaDefs/LuaEventsDef.h"
#include "Lua/LuaDefs/LuaFileDef.h"
#include "Lua/LuaDefs/LuaFontDef.h"
#include "Lua/LuaDefs/LuaGeometryDef.h"
#include "Lua/LuaDefs/LuaInputDef.h"
#include "Lua/LuaDefs/LuaLightDef.h"
#include "Lua/LuaDefs/LuaModelDef.h"
#include "Lua/LuaDefs/LuaNetworkDef.h"
#include "Lua/LuaDefs/LuaPhysicsDef.h"
#include "Lua/LuaDefs/LuaQuatDef.h"
#include "Lua/LuaDefs/LuaRenderingDef.h"
#include "Lua/LuaDefs/LuaRenderTargetDef.h"
#include "Lua/LuaDefs/LuaSceneDef.h"
#include "Lua/LuaDefs/LuaShaderDef.h"
#include "Lua/LuaDefs/LuaSoundDef.h"
#include "Lua/LuaDefs/LuaTextureDef.h"
#include "Lua/LuaDefs/LuaUtilsDef.h"
#include "Lua/LuaDefs/LuaVRDef.h"
#include "Utils/LuaUtils.h"

#define ROC_LUA_METATABLE "roc_mt"

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
    LuaDrawableDef::Init(m_vm);

    LuaAnimationDef::Init(m_vm);
    LuaCameraDef::Init(m_vm);
    LuaCollisionDef::Init(m_vm);
    LuaFileDef::Init(m_vm);
    LuaFontDef::Init(m_vm);
    LuaGeometryDef::Init(m_vm);
    LuaLightDef::Init(m_vm);
    LuaModelDef::Init(m_vm);
    LuaRenderTargetDef::Init(m_vm);
    LuaSceneDef::Init(m_vm);
    LuaShaderDef::Init(m_vm);
    LuaSoundDef::Init(m_vm);
    LuaTextureDef::Init(m_vm);

    LuaEventsDef::Init(m_vm);
    LuaInputDef::Init(m_vm);
    LuaNetworkDef::Init(m_vm);
    LuaPhysicsDef::Init(m_vm);
    LuaRenderingDef::Init(m_vm);
    LuaVRDef::Init(m_vm);

    LuaQuatDef::Init(m_vm);
    LuaUtilsDef::Init(m_vm);

    // Hidden metatable with weak values for elements
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
    delete m_eventManager;
    lua_close(m_vm);
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

void ROC::LuaManager::DoPulse()
{
    // Lua GC can't keep up to clean custom userdata on high FPS by itself, let's help it
    lua_gc(m_vm, LUA_GCSTEP, 0);
}

void ROC::LuaManager::CallFunction(const LuaFunction &f_func, const LuaArguments *f_args)
{
    if(f_func.IsValid() && (f_func.GetVM() == m_vm))
    {
        lua_rawgeti(m_vm, LUA_REGISTRYINDEX, f_func.GetReference());

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
                    Element *l_element = iter.GetElement();
                    LuaUtils::PushElementInMetatable(m_vm, ROC_LUA_METATABLE_USERDATA, l_element, l_element->GetElementTypeName().c_str());
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
}
