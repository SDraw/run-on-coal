#include "stdafx.h"

#include "Lua/LuaVM.h"
#include "Module/LuaModule.h"

#include "Lua/LuaFunction.h"
#include "Lua/LuaArgument.h"
#include "LuaDefs/LuaClientDef.h"
#include "LuaDefs/LuaElementDef.h"
#include "LuaDefs/LuaEventsDef.h"
#include "LuaDefs/LuaFileDef.h"
#include "LuaDefs/LuaUtilsDef.h"
#include "Utils/LuaUtils.h"

const char *LuaVM::ms_userdataMetatableName = "lm_ud";

LuaVM::LuaVM(LuaModule *f_module)
{
    m_luaModule = f_module;

    m_vm = luaL_newstate();
    luaL_requiref(m_vm, "_G", luaopen_base, 1);
    luaL_requiref(m_vm, "math", luaopen_math, 1);
    luaL_requiref(m_vm, "string", luaopen_string, 1);
    luaL_requiref(m_vm, "table", luaopen_table, 1);
    luaL_requiref(m_vm, "utf8", luaopen_utf8, 1);
    luaL_requiref(m_vm, "package", luaopen_package, 1);

    LuaElementDef::Init(m_vm);
    LuaClientDef::Init(m_vm);
    LuaFileDef::Init(m_vm);

    LuaEventsDef::Init(m_vm);
    LuaUtilsDef::Init(m_vm);

    // Hidden metatable with weak values for elements
    luaL_newmetatable(m_vm, "lm_mt");
    lua_pushvalue(m_vm, -1);
    lua_setfield(m_vm, -2, "__index");
    lua_pushstring(m_vm, "v");
    lua_setfield(m_vm, -2, "__mode");
    lua_pushboolean(m_vm, 0);
    lua_setfield(m_vm, -2, "__metatable");
    lua_setfield(m_vm, LUA_REGISTRYINDEX, ms_userdataMetatableName);
    lua_pop(m_vm, 1);
}

LuaVM::~LuaVM()
{
    lua_close(m_vm);
}

void LuaVM::LoadScript(const std::string &f_file)
{
    std::string l_log;
    if(luaL_loadfile(m_vm, f_file.c_str()) || lua_pcall(m_vm, 0, 0, 0))
    {
        l_log.append(lua_tostring(m_vm, -1));
        lua_pop(m_vm, 1);
    }
    else
    {
        l_log.append("Script '");
        l_log.append(f_file);
        l_log.append("' is loaded");
    }
    m_luaModule->GetEngineCore()->GetILogManager()->Log(l_log);
}

void LuaVM::DoPulse()
{
    // Lua GC can't keep up to clean custom userdata on high FPS by itself, let's help it
    lua_gc(m_vm, LUA_GCSTEP, 0);
}

void LuaVM::CallFunction(const LuaFunction &f_func, const ROC::ICustomArguments *f_args)
{
    if(f_func.IsValid() && (f_func.GetVM() == m_vm))
    {
        lua_rawgeti(m_vm, LUA_REGISTRYINDEX, f_func.GetReference());

        for(size_t i = 0U, j = f_args->GetArgumentsCount(); i < j; i++)
        {
            const ROC::ICustomArgument *l_customArgument = f_args->GetArgument(i);
            switch(l_customArgument->GetType())
            {
                case ROC::ICustomArgument::CAT_None:
                    lua_pushnil(m_vm);
                    break;
                case ROC::ICustomArgument::CAT_Boolean:
                    lua_pushboolean(m_vm, l_customArgument->GetBoolean());
                    break;
                case ROC::ICustomArgument::CAT_Integer:
                    lua_pushinteger(m_vm, l_customArgument->GetInteger());
                    break;
                case ROC::ICustomArgument::CAT_UInteger:
                    lua_pushinteger(m_vm, l_customArgument->GetUInteger());
                    break;
                case ROC::ICustomArgument::CAT_Double:
                    lua_pushnumber(m_vm, l_customArgument->GetDouble());
                    break;
                case ROC::ICustomArgument::CAT_Float:
                    lua_pushnumber(m_vm, l_customArgument->GetFloat());
                    break;
                case ROC::ICustomArgument::CAT_Pointer:
                    lua_pushlightuserdata(m_vm, l_customArgument->GetPointer());
                    break;
                case ROC::ICustomArgument::CAT_Element:
                {
                    ROC::IElement *l_element = reinterpret_cast<ROC::IElement*>(l_customArgument->GetElement());
                    LuaUtils::PushElementWithMetatable(m_vm, ms_userdataMetatableName, l_element, l_element->GetElementTypeName().c_str());
                } break;
                case ROC::ICustomArgument::CAT_String:
                {
                    const std::string &l_string = l_customArgument->GetString();
                    lua_pushlstring(m_vm, l_string.data(), l_string.size());
                } break;
            }
        }
        if(lua_pcall(m_vm, static_cast<int>(f_args->GetArgumentsCount()), 0, 0))
        {
            std::string l_log(lua_tostring(m_vm, -1));
            m_luaModule->GetEngineCore()->GetILogManager()->Log(l_log);
            lua_pop(m_vm, 1);
        }
    }
}

void LuaVM::CallFunction(const LuaFunction &f_func, const std::vector<LuaArgument> &f_args)
{
    if(f_func.IsValid() && (f_func.GetVM() == m_vm))
    {
        lua_rawgeti(m_vm, LUA_REGISTRYINDEX, f_func.GetReference());

        for(const auto &iter : f_args)
        {
            switch(iter.GetType())
            {
                case LuaArgument::LAT_Nil:
                    lua_pushnil(m_vm);
                    break;
                case LuaArgument::LAT_Boolean:
                    lua_pushboolean(m_vm, iter.GetBoolean());
                    break;
                case LuaArgument::LAT_Integer:
                    lua_pushinteger(m_vm, iter.GetInteger());
                    break;
                case LuaArgument::LAT_Number:
                    lua_pushnumber(m_vm, iter.GetNumber());
                    break;
                case LuaArgument::LAT_LightUserdata:
                    lua_pushlightuserdata(m_vm, iter.GetLightUserdata());
                    break;
                case LuaArgument::LAT_Userdata:
                    LuaUtils::PushElementWithMetatable(m_vm, ms_userdataMetatableName, iter.GetLightUserdata(), iter.GetString().c_str());
                    break;
                case LuaArgument::LAT_String:
                {
                    const std::string &l_string = iter.GetString();
                    lua_pushlstring(m_vm, l_string.data(), l_string.size());
                } break;
            }
        }
        if(lua_pcall(m_vm, static_cast<int>(f_args.size()), 0, 0))
        {
            std::string l_log(lua_tostring(m_vm, -1));
            m_luaModule->GetEngineCore()->GetILogManager()->Log(l_log);
            lua_pop(m_vm, 1);
        }
    }
}
