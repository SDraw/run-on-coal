#include "stdafx.h"
#include "Lua/LuaArguments.h"

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
                lua_pushlstring(f_vm, iter->m_value.m_string->data(), iter->m_value.m_string->size());
                break;
        }
    }
}


ROC::LuaArguments::LuaArgument::LuaArgument(int f_val)
{
    m_value.m_int = f_val;
    m_type = Integer;
}
ROC::LuaArguments::LuaArgument::LuaArgument(double f_val)
{
    m_value.m_double = f_val;
    m_type = Double;
}
ROC::LuaArguments::LuaArgument::LuaArgument(float f_val)
{
    m_value.m_float = f_val;
    m_type = Float;
}
ROC::LuaArguments::LuaArgument::LuaArgument(void *f_val)
{
    m_value.m_pointer = f_val;
    m_type = Pointer;
}
ROC::LuaArguments::LuaArgument::LuaArgument(std::string *f_val)
{
    m_value.m_string = f_val;
    m_type = String;
}
