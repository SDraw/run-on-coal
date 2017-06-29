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
void ROC::LuaArguments::PushArgument(void *f_val, const std::string &f_name)
{
    m_dummyData.SetElement(f_val,f_name);
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
