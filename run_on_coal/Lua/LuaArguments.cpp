#include "stdafx.h"
#include "LuaArguments.h"

ROC::LuaArguments::LuaArguments()
{
    m_argCount = 0;
}
ROC::LuaArguments::~LuaArguments()
{
    Clear();
}

void ROC::LuaArguments::Clear()
{
    m_vArgs.clear();
    m_argCount = 0;
}

void ROC::LuaArguments::ProccessArguments(lua_State *f_vm)
{
    for(auto iter = m_vArgs.begin(), iterEnd = m_vArgs.end(); iter != iterEnd; ++iter)
    {
        switch(iter->m_type)
        {
            case LuaArgument::ArgumentType::Integer:
                lua_pushinteger(f_vm, iter->m_value.m_int);
                break;
            case LuaArgument::ArgumentType::Double:
                lua_pushnumber(f_vm, iter->m_value.m_double);
                break;
            case LuaArgument::ArgumentType::Float:
                lua_pushnumber(f_vm, iter->m_value.m_float);
                break;
            case LuaArgument::ArgumentType::Pointer:
                lua_pushlightuserdata(f_vm, iter->m_value.m_pointer);
                break;
            case LuaArgument::ArgumentType::String:
                lua_pushstring(f_vm, iter->m_value.m_string->c_str());
                break;
        }
    }
}