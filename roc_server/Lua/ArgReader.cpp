#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/Element.h"
#include "Lua/ArgReader.h"

ROC::ArgReader::ArgReader(lua_State *f_vm)
{
    m_vm = f_vm;
    m_currentArg = 1;
    m_argCount = lua_gettop(m_vm);
    m_returnValue = 0;
    m_hasErrors = false;
}
ROC::ArgReader::~ArgReader()
{
}

void ROC::ArgReader::ReadBoolean(bool &f_val)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_isboolean(m_vm, m_currentArg))
            {
                f_val = (lua_toboolean(m_vm, m_currentArg) == 1);
                m_currentArg++;
            }
            else
            {
                m_error.assign("Expected boolean");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.assign("Not enough arguments");
            m_hasErrors = true;
        }
    }
}

void ROC::ArgReader::ReadText(std::string &f_val)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_isstring(m_vm, m_currentArg))
            {
                size_t l_size;
                const char *l_string = lua_tolstring(m_vm, m_currentArg, &l_size);
                f_val.assign(l_string,l_size);
                m_currentArg++;
            }
            else
            {
                m_error.assign("Expected string");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.assign("Not enough arguments");
            m_hasErrors = true;
        }
    }
}
void ROC::ArgReader::ReadFunction(int &f_val, void *&f_pointer)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_isfunction(m_vm, m_currentArg))
            {
                f_pointer = const_cast<void*>(lua_topointer(m_vm, m_currentArg));
                lua_settop(m_vm, m_currentArg);
                f_val = luaL_ref(m_vm, LUA_REGISTRYINDEX);
                lua_insert(m_vm, m_currentArg);
                m_currentArg++;
            }
            else
            {
                m_error.assign("Expected function");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.assign("Not enough arguments");
            m_hasErrors = true;
        }
    }
}
void ROC::ArgReader::ReadFunction(void *&f_pointer)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_isfunction(m_vm, m_currentArg))
            {
                f_pointer = const_cast<void*>(lua_topointer(m_vm, m_currentArg));
                m_currentArg++;
            }
            else
            {
                m_error.assign("Expected function");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.assign("Not enough arguments");
            m_hasErrors = true;
        }
    }
}

void ROC::ArgReader::ReadNextBoolean(bool &f_val)
{
    if(!m_hasErrors && (m_currentArg <= m_argCount))
    {
        if(lua_isboolean(m_vm, m_currentArg))
        {
            f_val = (lua_toboolean(m_vm, m_currentArg) == 1);
            m_currentArg++;
        }
    }
}
void ROC::ArgReader::ReadNextText(std::string &f_val)
{
    if(!m_hasErrors && (m_currentArg <= m_argCount))
    {
        if(lua_isstring(m_vm, m_currentArg))
        {
            size_t l_size;
            const char *l_string = lua_tolstring(m_vm, m_currentArg, &l_size);
            f_val.assign(l_string,l_size);
            m_currentArg++;
        }
    }
}

void ROC::ArgReader::ReadMatrix(float *f_val, int f_size)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_istable(m_vm, m_currentArg))
            {
                lua_settop(m_vm, m_currentArg);
                for(int i = 0; i < f_size; i++)
                {
                    lua_pushnumber(m_vm, i + 1);
                    lua_gettable(m_vm, -2);
                    if(lua_isnumber(m_vm, -1))
                    {
                        f_val[i] = static_cast<float>(lua_tonumber(m_vm, -1));
                        lua_pop(m_vm, 1);
                        if(std::isnan(f_val[i]) || std::isinf(f_val[i]))
                        {
                            m_error.assign("Got NaN/Inf at table index ");
                            m_error.append(std::to_string(i));
                            break;
                        }
                    }
                    else
                    {
                        lua_pop(m_vm, 1);
                        m_error.assign("Table index ");
                        m_error.append(std::to_string(i));
                        m_error.append(" isn't a number");
                        break;
                    }
                }
                lua_insert(m_vm, m_currentArg);
                if(m_error.empty()) m_currentArg++;
                else m_hasErrors = true;
            }
            else
            {
                m_error.assign("Expected table");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.assign("Not enough arguments");
            m_hasErrors = true;
        }
    }
}
void ROC::ArgReader::ReadTableTexts(std::vector<std::string> &f_vec, int f_size)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_istable(m_vm, m_currentArg))
            {
                lua_settop(m_vm, m_currentArg);
                for(int i = 0; i < f_size; i++)
                {
                    lua_pushnumber(m_vm, i + 1);
                    lua_gettable(m_vm, -2);
                    if(lua_isstring(m_vm, -1))
                    {
                        size_t l_size;
                        const char *l_string = lua_tolstring(m_vm, -1, &l_size);
                        f_vec.push_back(std::string(l_string,l_size));
                        lua_pop(m_vm, 1);
                    }
                    else
                    {
                        m_error.assign("Not enough table values");
                        lua_pop(m_vm, 1);
                        break;
                    }
                }
                lua_insert(m_vm, m_currentArg);
                if(m_error.empty()) m_currentArg++;
                else m_hasErrors = true;
            }
            else
            {
                m_error.assign("Expected table");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.assign("Not enough arguments");
            m_hasErrors = true;
        }
    }
}

void ROC::ArgReader::PushBoolean(bool f_val)
{
    lua_pushboolean(m_vm, f_val);
    m_returnValue++;
}
void ROC::ArgReader::PushNumber(lua_Number f_val)
{
    lua_pushnumber(m_vm, f_val);
    m_returnValue++;
}
void ROC::ArgReader::PushInteger(lua_Integer f_val)
{
    lua_pushinteger(m_vm, f_val);
    m_returnValue++;
}
void ROC::ArgReader::PushText(const std::string &f_val)
{
    lua_pushlstring(m_vm, f_val.data(), f_val.size());
    m_returnValue++;
}
void ROC::ArgReader::PushPointer(void *f_val)
{
    lua_pushlightuserdata(m_vm, f_val);
    m_returnValue++;
}
void ROC::ArgReader::PushMatrix(float *f_val, int f_size)
{
    lua_newtable(m_vm);
    for(int i = 0; i < f_size; i++)
    {
        lua_pushnumber(m_vm, i + 1);
        lua_pushnumber(m_vm, f_val[i]);
        lua_settable(m_vm, -3);
    }
    m_returnValue++;
}

bool ROC::ArgReader::HasErrors()
{
    if(m_hasErrors)
    {
        std::string l_log("Warning at ");
        lua_Debug l_ar;
        lua_getstack(m_vm, 1, &l_ar);
        lua_getinfo(m_vm, "nSl", &l_ar);
        l_log.append(l_ar.source);
        l_log.push_back(':');
        l_log.append(std::to_string(l_ar.currentline));
        l_log.append(": ");
        l_log.append(m_error);
        l_log.append(" at argument ");
        l_log.append(std::to_string(m_currentArg));
        LuaManager::GetCore()->GetLogManager()->Log(l_log);
    }
    return m_hasErrors;
}