#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/LogManager.h"
#include "Managers/MemoryManager.h"
#include "Lua/ArgReader.h"

ROC::ArgReader::ArgReader(lua_State *f_vm, Core *f_core)
{
    m_pVM = f_vm;
    m_core = f_core;
    m_hasErrors = false;
    m_iArgIndex = 1;
    m_iArgNum = lua_gettop(m_pVM);
}
ROC::ArgReader::~ArgReader()
{
}

void ROC::ArgReader::DecreaseArguments(int f_args)
{
    if(m_iArgNum > f_args) 
    {
        lua_pop(m_pVM,m_iArgNum-f_args);
        m_iArgNum = f_args;
    }
}

void ROC::ArgReader::ReadBoolean(bool &f_val)
{
    if(m_hasErrors) return;
    if(m_iArgNum < m_iArgIndex)
    {
        m_error.append("Not enough arguments");
        m_hasErrors = true;
        return;
    }
    if(!lua_isboolean(m_pVM,m_iArgIndex))
    {
        m_error.append("Expected boolean");
        m_hasErrors = true;
        return;
    }
    f_val = (lua_toboolean(m_pVM,m_iArgIndex) == 1);
    m_iArgIndex++;
}
void ROC::ArgReader::ReadNumber(lua_Number &f_val)
{
    if(m_hasErrors) return;
    if(m_iArgNum < m_iArgIndex)
    {
        m_error.append("Not enough arguments");
        m_hasErrors = true;
        return;
    }
    if(!lua_isnumber(m_pVM,m_iArgIndex))
    {
        m_error.append("Expected number");
        m_hasErrors = true;
        return;
    }
    lua_Number l_val = lua_tonumber(m_pVM,m_iArgIndex);
    if(std::isnan(l_val) || std::isinf(l_val))
    {
        m_error.append("Got NaN/Inf");
        m_hasErrors = true;
        return;
    }
    f_val = l_val;
    m_iArgIndex++;
}
void ROC::ArgReader::ReadInteger(LUA_INTEGER &f_val)
{
    if(m_hasErrors) return;
    if(m_iArgNum < m_iArgIndex)
    {
        m_error.append("Not enough arguments");
        m_hasErrors = true;
        return;
    }
    if(!lua_isinteger(m_pVM,m_iArgIndex))
    {
        m_error.append("Expected integer");
        m_hasErrors = true;
        return;
    }
    f_val = lua_tointeger(m_pVM,m_iArgIndex);
    m_iArgIndex++;
}
void ROC::ArgReader::ReadText(std::string &f_val)
{
    if(m_hasErrors) return;
    if(m_iArgNum < m_iArgIndex)
    {
        m_error.append("Not enough arguments");
        m_hasErrors = true;
        return;
    }
    if(!lua_isstring(m_pVM,m_iArgIndex))
    {
        m_error.append("Expected string");
        m_hasErrors = true;
        return;
    }
    f_val.append(lua_tostring(m_pVM,m_iArgIndex));
    m_iArgIndex++;
}
void ROC::ArgReader::ReadUserdata(void **f_val, unsigned int f_type)
{
    if(m_hasErrors) return;
    if(m_iArgNum < m_iArgIndex)
    {
        m_error.append("Not enough arguments");
        m_hasErrors = true;
        return;
    }
    if(!lua_islightuserdata(m_pVM,m_iArgIndex))
    {
        m_error.append("Expected userdata");
        m_hasErrors = true;
        return;
    }
    void *a = (void*)lua_topointer(m_pVM,m_iArgIndex);
    if(!m_core->GetMemoryManager()->CheckMemoryPointer(a,f_type))
    {
        m_error.append("Invalid userdata");
        m_hasErrors = true;
        return;
    }
    *f_val = a;
    m_iArgIndex++;
}
void ROC::ArgReader::ReadPointer(void **f_val)
{
    if(m_hasErrors) return;
    if(m_iArgNum < m_iArgIndex)
    {
        m_error.append("Not enough arguments");
        m_hasErrors = true;
        return;
    }
    if(!lua_islightuserdata(m_pVM,m_iArgIndex))
    {
        m_error.append("Expected userdata/pointer");
        m_hasErrors = true;
        return;
    }
    *f_val = (void*)lua_topointer(m_pVM,m_iArgIndex);
    m_iArgIndex++;
}
void ROC::ArgReader::ReadFunction(int &f_val,void **f_pointer)
{
    if(m_hasErrors) return;
    if(m_iArgNum < m_iArgIndex)
    {
        m_error.append("Not enough arguments");
        m_hasErrors = true;
        return;
    }
    if(!lua_isfunction(m_pVM,m_iArgIndex))
    {
        m_error.append("Expected function");
        m_hasErrors = true;
        return;
    }
    *f_pointer = (void*)lua_topointer(m_pVM,m_iArgIndex);
    f_val = luaL_ref(m_pVM,LUA_REGISTRYINDEX);
    m_iArgIndex++;
}

void ROC::ArgReader::ReadNextBoolean(bool &f_val)
{
    if(m_hasErrors) return;
    if(m_iArgNum < m_iArgIndex) return;
    if(!lua_isboolean(m_pVM,m_iArgIndex)) return;
    f_val = (lua_toboolean(m_pVM,m_iArgIndex) == 1);
    m_iArgIndex++;
}
void ROC::ArgReader::ReadNextNumber(lua_Number &f_val)
{
    if(m_hasErrors) return;
    if(m_iArgNum < m_iArgIndex) return;
    if(!lua_isnumber(m_pVM,m_iArgIndex)) return;
    lua_Number l_val = lua_tonumber(m_pVM,m_iArgIndex);
    if(std::isnan(l_val) || std::isinf(l_val)) return;
    f_val = l_val;
    m_iArgIndex++;
}
void ROC::ArgReader::ReadNextInteger(LUA_INTEGER &f_val)
{
    if(m_hasErrors) return;
    if(m_iArgNum < m_iArgIndex) return;
    if(!lua_isinteger(m_pVM,m_iArgIndex)) return;
    f_val = lua_tointeger(m_pVM,m_iArgIndex);
    m_iArgIndex++;
}
void ROC::ArgReader::ReadNextText(std::string &f_val)
{
    if(m_hasErrors) return;
    if(m_iArgNum < m_iArgIndex) return;
    if(!lua_isstring(m_pVM,m_iArgIndex)) return;
    f_val.append(lua_tostring(m_pVM,m_iArgIndex));
    m_iArgIndex++;
}
void ROC::ArgReader::ReadNextUserdata(void **f_val, unsigned int f_type)
{
    if(m_hasErrors) return;
    if(m_iArgNum < m_iArgIndex) return;
    if(!lua_islightuserdata(m_pVM,m_iArgIndex))
    {
        m_error.append("Expected userdata");
        m_hasErrors = true;
        return;
    }
    void *a = (void*)lua_topointer(m_pVM,m_iArgIndex);
    if(!m_core->GetMemoryManager()->CheckMemoryPointer(a,f_type))
    {
        m_error.append("Invalid userdata");
        m_hasErrors = true;
        return;
    }
    *f_val = a;
    m_iArgIndex++;
}
void ROC::ArgReader::ReadNextPointer(void **f_val)
{
    if(m_hasErrors) return;
    if(m_iArgNum < m_iArgIndex) return;
    if(!lua_islightuserdata(m_pVM,m_iArgIndex))
    {
        m_error.append("Expected userdata");
        m_hasErrors = true;
        return;
    }
    *f_val = (void*)lua_topointer(m_pVM,m_iArgIndex);
    m_iArgIndex++;
}

void ROC::ArgReader::ReadTableNumbers(std::vector<double> &f_vec, int f_size)
{
    if(m_hasErrors) return;
    if(m_iArgNum < m_iArgIndex)
    {
        m_error.append("Not enough arguments");
        m_hasErrors = true;
        return;
    }
    if(!lua_istable(m_pVM,m_iArgIndex))
    {
        m_error.append("Expected table");
        m_hasErrors = true;
        return;
    }
    for(int i=0; i < f_size; i++)
    {
        lua_pushnumber(m_pVM,i+1);
        lua_gettable(m_pVM,-2);
        if(!lua_isnumber(m_pVM,-1))
        {
            m_error.append("Not enough table values");
            m_hasErrors = true;
            return;
        }
        lua_Number l_val = lua_tonumber(m_pVM,-1);
        if(std::isnan(l_val) || std::isinf(l_val))
        {
            m_error.append("Got NaN or Inf");
            m_hasErrors = true;
            return;
        }
        f_vec.push_back(l_val);
        lua_pop(m_pVM,1);
    }
    m_iArgIndex++;
}

void ROC::ArgReader::ReadTableTexts(std::vector<std::string> &f_vec, int f_size)
{
    if(m_hasErrors) return;
    if(m_iArgNum < m_iArgIndex)
    {
        m_error.append("Not enough arguments");
        m_hasErrors = true;
        return;
    }
    if(!lua_istable(m_pVM,m_iArgIndex))
    {
        m_error.append("Expected table");
        m_hasErrors = true;
        return;
    }
    for(int i=0; i < f_size; i++)
    {
        lua_pushnumber(m_pVM,i+1);
        lua_gettable(m_pVM,-2);
        if(!lua_isstring(m_pVM,-1))
        {
            m_error.append("Not enough table values");
            m_hasErrors = true;
            return;
        }
        f_vec.push_back(lua_tostring(m_pVM,-1));
        lua_pop(m_pVM,1);
    }
    m_iArgIndex++;
}

bool ROC::ArgReader::HasErrors()
{
    if(m_hasErrors)
    {
        std::string l_log("Warning at ");
        lua_Debug l_ar;
        lua_getstack(m_pVM, 1, &l_ar);
        lua_getinfo(m_pVM, "nSl", &l_ar);
        l_log.append(l_ar.source);
        l_log.append(":");
        l_log.append(std::to_string(l_ar.currentline));
        l_log.append(": ");
        l_log.append(m_error);
        l_log.append(" at argument ");
        l_log.append(std::to_string(m_iArgIndex));
        m_core->GetLogManager()->Log(l_log);
    }
    return m_hasErrors;
}
