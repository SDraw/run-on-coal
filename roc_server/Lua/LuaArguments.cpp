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

void ROC::LuaArguments::PushArgument(bool f_val)
{
    m_dummyData.SetBoolean(f_val);
    m_vArgs.push_back(m_dummyData);
}
void ROC::LuaArguments::PushArgument(int f_val)
{
    m_dummyData.SetInteger(f_val);
    m_vArgs.push_back(m_dummyData);
}
void ROC::LuaArguments::PushArgument(double f_val)
{
    m_dummyData.SetDouble(f_val);
    m_vArgs.push_back(m_dummyData);
}
void ROC::LuaArguments::PushArgument(float f_val)
{
    m_dummyData.SetFloat(f_val);
    m_vArgs.push_back(m_dummyData);
}
void ROC::LuaArguments::PushArgument(void *f_val)
{
    m_dummyData.SetPointer(f_val);
    m_vArgs.push_back(m_dummyData);
}
void ROC::LuaArguments::PushArgument(const std::string &f_val)
{
    m_dummyData.SetString(f_val);
    m_vArgs.push_back(m_dummyData);
    m_dummyData.ClearString();
}
void ROC::LuaArguments::PushArgument(const char *f_val, size_t f_size)
{
    m_dummyData.SetString(f_val, f_size);
    m_vArgs.push_back(m_dummyData);
    m_dummyData.ClearString();
}

void ROC::LuaArguments::ProccessArguments(lua_State *f_vm)
{
    for(auto &iter : m_vArgs)
    {
        switch(iter.GetType())
        {
            case CustomData::DataType::Boolean:
                lua_pushboolean(f_vm, iter.GetBoolean());
                break;
            case CustomData::DataType::Integer:
                lua_pushinteger(f_vm, iter.GetInteger());
                break;
            case CustomData::DataType::Double:
                lua_pushnumber(f_vm, iter.GetDouble());
                break;
            case CustomData::DataType::Float:
                lua_pushnumber(f_vm, iter.GetFloat());
                break;
            case CustomData::DataType::Pointer:
                lua_pushlightuserdata(f_vm, iter.GetPointer());
                break;
            case CustomData::DataType::String:
            {
                const std::string &l_string = iter.GetString();
                lua_pushlstring(f_vm, l_string.data(), l_string.size());
            } break;
        }
    }
}
