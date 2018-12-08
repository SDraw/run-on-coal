#include "stdafx.h"

#include "Managers/LuaManager.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Lua/LuaArguments.h"

#include "Managers/ConfigManager.h"
#include "Managers/LogManager.h"
#include "Elements/Element.h"
#include "Lua/LuaDefs/LuaClientDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Lua/LuaDefs/LuaEventsDef.h"
#include "Lua/LuaDefs/LuaFileDef.h"
#include "Lua/LuaDefs/LuaUtilsDef.h"
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
    delete m_eventManager;
    lua_close(m_vm);
}

void ROC::LuaManager::LoadDefaultScripts()
{
    // Load default scripts
    std::string l_metaPath(m_core->GetConfigManager()->GetScriptsDirectory());
    l_metaPath.append("/meta.xml");
    pugi::xml_document *l_meta = new pugi::xml_document();
    if(l_meta->load_file(l_metaPath.c_str()))
    {
        pugi::xml_node l_metaRoot = l_meta->child("meta");
        if(l_metaRoot)
        {
            for(pugi::xml_node l_node = l_metaRoot.child("script"); l_node; l_node = l_node.next_sibling("script"))
            {
                pugi::xml_attribute l_attrib = l_node.attribute("src");
                if(l_attrib)
                {
                    std::string l_path(m_core->GetConfigManager()->GetScriptsDirectory());
                    l_path.push_back('/');
                    l_path.append(l_attrib.as_string());
                    LoadScript(l_path);
                }
            }
        }
    }
    else
    {
        std::string l_error("Unable to find '");
        l_error.append(l_metaPath);
        l_error.push_back('\'');
        m_core->GetLogManager()->Log(l_error);
    }
    delete l_meta;
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
    // Lua GC can't keep up to clean custom userdata on high ticks by itself, let's help it
    lua_gc(m_vm, LUA_GCSTEP, 0);
}

void ROC::LuaManager::CallFunction(const LuaFunction &f_func, const LuaArguments *f_args)
{
    if(f_func.IsValid() && f_func.GetVM() == m_vm)
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
