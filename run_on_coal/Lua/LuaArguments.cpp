#include "stdafx.h"
#include "Lua/LuaArguments.h"

ROC::LuaArguments::LuaArguments()
{
}
ROC::LuaArguments::~LuaArguments()
{
    m_vArgs.clear();
}

void ROC::LuaArguments::Clear()
{
    m_vArgs.clear();
}

void ROC::LuaArguments::PushArgument(int f_val)
{
    m_dummyArgument.m_int = f_val;
    m_dummyArgument.m_type = LuaArgument::ArgumentType::Integer;
    m_vArgs.push_back(m_dummyArgument);
}
void ROC::LuaArguments::PushArgument(double f_val)
{
    m_dummyArgument.m_double = f_val;
    m_dummyArgument.m_type = LuaArgument::ArgumentType::Double;
    m_vArgs.push_back(m_dummyArgument);
}
void ROC::LuaArguments::PushArgument(float f_val)
{
    m_dummyArgument.m_float = f_val;
    m_dummyArgument.m_type = LuaArgument::ArgumentType::Float;
    m_vArgs.push_back(m_dummyArgument);
}
void ROC::LuaArguments::PushArgument(void *f_val)
{
    m_dummyArgument.m_pointer = f_val;
    m_dummyArgument.m_type = LuaArgument::ArgumentType::Pointer;
    m_vArgs.push_back(m_dummyArgument);
}
void ROC::LuaArguments::PushArgument(const std::string &f_val)
{
    std::string &l_dummyString = m_dummyArgument.m_string;
    l_dummyString.assign(f_val);
    m_dummyArgument.m_type = LuaArgument::ArgumentType::String;
    m_vArgs.push_back(m_dummyArgument);
    l_dummyString.clear();
}
void ROC::LuaArguments::PushArgument(const char *f_val, size_t f_size)
{
    std::string &l_dummyString = m_dummyArgument.m_string;
    l_dummyString.assign(f_val,f_size);
    m_dummyArgument.m_type = LuaArgument::ArgumentType::String;
    m_vArgs.push_back(m_dummyArgument);
    l_dummyString.resize(0U);
}

void ROC::LuaArguments::ProccessArguments(lua_State *f_vm)
{
    for(auto &iter : m_vArgs)
    {
        switch(iter.m_type)
        {
            case LuaArgument::ArgumentType::Integer:
                lua_pushinteger(f_vm, iter.m_int);
                break;
            case LuaArgument::ArgumentType::Double:
                lua_pushnumber(f_vm, iter.m_double);
                break;
            case LuaArgument::ArgumentType::Float:
                lua_pushnumber(f_vm, iter.m_float);
                break;
            case LuaArgument::ArgumentType::Pointer:
                lua_pushlightuserdata(f_vm, iter.m_pointer);
                break;
            case LuaArgument::ArgumentType::String:
                lua_pushlstring(f_vm, iter.m_string.data(), iter.m_string.size());
                break;
        }
    }
}
