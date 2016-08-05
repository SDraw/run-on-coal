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
    for(auto iter : m_vArgs)
    {
        switch(iter.second)
        {
            case ArgumentType::Integer:
                delete static_cast<int*>(iter.first);
                break;
            case ArgumentType::Double:
                delete static_cast<double*>(iter.first);
                break;
            case ArgumentType::String:
                delete static_cast<std::string*>(iter.first);
                break;
            case ArgumentType::Float:
                delete static_cast<float*>(iter.first);
                break;
        }
    }
    m_vArgs.clear();
    m_argCount = 0;
}

void ROC::LuaArguments::PushArgument(int f_val)
{
    int *l_temp = new int(f_val);
    m_vArgs.push_back(std::pair<void*,ArgumentType>(l_temp,ArgumentType::Integer));
    m_argCount++;
}

void ROC::LuaArguments::PushArgument(double f_val)
{
    double *l_temp = new double(f_val);
    m_vArgs.push_back(std::pair<void*,ArgumentType>(l_temp,ArgumentType::Double));
    m_argCount++;
}

void ROC::LuaArguments::PushArgument(float f_val)
{
    float *l_temp = new float(f_val);
    m_vArgs.push_back(std::pair<void*,ArgumentType>(l_temp,ArgumentType::Float));
    m_argCount++;
}

void ROC::LuaArguments::PushArgument(void *f_val)
{
    m_vArgs.push_back(std::pair<void*,ArgumentType>(f_val,ArgumentType::Pointer));
    m_argCount++;
}

void ROC::LuaArguments::PushArgument(std::string &f_arg)
{
    std::string *l_temp = new std::string(f_arg);
    m_vArgs.push_back(std::pair<void*,ArgumentType>(l_temp,ArgumentType::String));
    m_argCount++;
}

void ROC::LuaArguments::ProccessArguments(lua_State *f_vm)
{
    for(auto iter : m_vArgs)
    {
        switch(iter.second)
        {
            case ArgumentType::Integer:
                lua_pushinteger(f_vm,*static_cast<int*>(iter.first));
                break;
            case ArgumentType::Double:
                lua_pushnumber(f_vm,*static_cast<double*>(iter.first));
                break;
            case ArgumentType::Float:
                lua_pushnumber(f_vm,*static_cast<float*>(iter.first));
                break;
            case ArgumentType::Pointer:
                lua_pushlightuserdata(f_vm,iter.first);
                break;
            case ArgumentType::String:
                lua_pushstring(f_vm,static_cast<std::string*>(iter.first)->c_str());
                break;
        }
    }
}
