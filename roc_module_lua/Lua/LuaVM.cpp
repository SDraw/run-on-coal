#include "stdafx.h"

#include "Lua/LuaVM.h"
#include "Lua/LuaFunction.h"
#include "Lua/LuaArgument.h"
#include "Core/Core.h"

#include "Lua/LuaDefs/LuaAnimationDef.h"
#include "Lua/LuaDefs/LuaCameraDef.h"
#include "Lua/LuaDefs/LuaCollidableDef.h"
#include "Lua/LuaDefs/LuaCollisionDef.h"
#include "Lua/LuaDefs/LuaDrawableDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Lua/LuaDefs/LuaEventsDef.h"
#include "Lua/LuaDefs/LuaFontDef.h"
#include "Lua/LuaDefs/LuaGeometryDef.h"
#include "Lua/LuaDefs/LuaInputDef.h"
#include "Lua/LuaDefs/LuaLightDef.h"
#include "Lua/LuaDefs/LuaModelDef.h"
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

const char *LuaVM::ms_userdataMetatableName = "lm_ud";

LuaVM::LuaVM(Core *f_module)
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
    LuaCollidableDef::Init(m_vm);
    LuaDrawableDef::Init(m_vm);

    LuaAnimationDef::Init(m_vm);
    LuaCameraDef::Init(m_vm);
    LuaCollisionDef::Init(m_vm);
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
    LuaPhysicsDef::Init(m_vm);
    LuaRenderingDef::Init(m_vm);
    LuaVRDef::Init(m_vm);

    LuaQuatDef::Init(m_vm);
    LuaUtilsDef::Init(m_vm);

    // Hidden metatable with weak values for elements
    luaL_newmetatable(m_vm, "lm_mt"); // Name
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
        l_log.assign("Script '");
        l_log.append(f_file);
        l_log.append("' is loaded");
    }
    m_luaModule->GetEngineICore()->GetILogManager()->Log(l_log);
}

void LuaVM::DoPulse()
{
    // Lua GC can't keep up to clean custom userdata on high FPS by itself, let's help it
    lua_gc(m_vm, LUA_GCSTEP, 0); // Does it even work?
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
            m_luaModule->GetEngineICore()->GetILogManager()->Log(l_log);
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
            m_luaModule->GetEngineICore()->GetILogManager()->Log(l_log);
            lua_pop(m_vm, 1);
        }
    }
}
