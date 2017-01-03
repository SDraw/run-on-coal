#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/LogManager.h"
#include "Managers/MemoryManager.h"
#include "Lua/ArgReader.h"

ROC::ArgReader::ArgReader(lua_State *f_vm, Core *f_core)
{
    m_vm = f_vm;
    m_core = f_core;
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
                m_error.append("Expected boolean");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.append("Not enough arguments");
            m_hasErrors = true;
        }
    }
}
void ROC::ArgReader::ReadNumber(lua_Number &f_val)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_isnumber(m_vm, m_currentArg))
            {
                f_val = lua_tonumber(m_vm, m_currentArg);
                if(!std::isnan(f_val) && !std::isinf(f_val)) m_currentArg++;
                else
                {
                    m_error.append("Got NaN/Inf");
                    m_hasErrors = true;
                }
            }
            else
            {
                m_error.append("Expected number");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.append("Not enough arguments");
            m_hasErrors = true;
        }
    }
}
void ROC::ArgReader::ReadNumber(float &f_val)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_isnumber(m_vm, m_currentArg))
            {
                f_val = static_cast<float>(lua_tonumber(m_vm, m_currentArg));
                if(!std::isnan(f_val) && !std::isinf(f_val)) m_currentArg++;
                else
                {
                    m_error.append("Got NaN/Inf");
                    m_hasErrors = true;
                }
            }
            else
            {
                m_error.append("Expected number");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.append("Not enough arguments");
            m_hasErrors = true;
        }
    }
}
void ROC::ArgReader::ReadInteger(lua_Integer &f_val)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_isinteger(m_vm, m_currentArg))
            {
                f_val = lua_tointeger(m_vm, m_currentArg);
                m_currentArg++;
            }
            else
            {
                m_error.append("Expected integer");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.append("Not enough arguments");
            m_hasErrors = true;
        }
    }
}
void ROC::ArgReader::ReadInteger(int &f_val)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_isinteger(m_vm, m_currentArg))
            {
                f_val = static_cast<int>(lua_tointeger(m_vm, m_currentArg));
                m_currentArg++;
            }
            else
            {
                m_error.append("Expected integer");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.append("Not enough arguments");
            m_hasErrors = true;
        }
    }
}
void ROC::ArgReader::ReadInteger(unsigned int &f_val)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_isinteger(m_vm, m_currentArg))
            {
                f_val = static_cast<unsigned int>(lua_tointeger(m_vm, m_currentArg));
                m_currentArg++;
            }
            else
            {
                m_error.append("Expected integer");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.append("Not enough arguments");
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
                f_val.append(lua_tostring(m_vm, m_currentArg));
                m_currentArg++;
            }
            else
            {
                m_error.append("Expected string");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.append("Not enough arguments");
            m_hasErrors = true;
        }
    }
}
void ROC::ArgReader::ReadUserdata(void **f_val, unsigned int f_type)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_islightuserdata(m_vm, m_currentArg))
            {
                void *a = const_cast<void*>(lua_topointer(m_vm, m_currentArg));
                if(m_core->GetMemoryManager()->CheckMemoryPointer(a, f_type))
                {
                    *f_val = a;
                    m_currentArg++;
                }
                else
                {
                    m_error.append("Invalid userdata");
                    m_hasErrors = true;
                }
            }
            else
            {
                m_error.append("Expected userdata");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.append("Not enough arguments");
            m_hasErrors = true;
        }
    }
}
void ROC::ArgReader::ReadPointer(void **f_val)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_islightuserdata(m_vm, m_currentArg))
            {
                *f_val = const_cast<void*>(lua_topointer(m_vm, m_currentArg));
                m_currentArg++;
            }
            else
            {
                m_error.append("Expected userdata/pointer");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.append("Not enough arguments");
            m_hasErrors = true;
        }
    }
}
void ROC::ArgReader::ReadFunction(int &f_val, void **f_pointer)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_isfunction(m_vm, m_currentArg))
            {
                *f_pointer = const_cast<void*>(lua_topointer(m_vm, m_currentArg));
                lua_settop(m_vm, m_currentArg);
                f_val = luaL_ref(m_vm, LUA_REGISTRYINDEX);
                lua_insert(m_vm, m_currentArg);
                m_currentArg++;
            }
            else
            {
                m_error.append("Expected function");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.append("Not enough arguments");
            m_hasErrors = true;
        }
    }
}
void ROC::ArgReader::ReadFunctionPointer(void **f_pointer)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_isfunction(m_vm, m_currentArg))
            {
                *f_pointer = const_cast<void*>(lua_topointer(m_vm, m_currentArg));
                m_currentArg++;
            }
            else
            {
                m_error.append("Expected function");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.append("Not enough arguments");
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
void ROC::ArgReader::ReadNextNumber(lua_Number &f_val)
{
    if(!m_hasErrors && (m_currentArg <= m_argCount))
    {
        if(lua_isnumber(m_vm, m_currentArg))
        {
            lua_Number l_val = lua_tonumber(m_vm, m_currentArg);
            if(!std::isnan(l_val) && !std::isinf(l_val))
            {
                f_val = l_val;
                m_currentArg++;
            }
        }
    }
}
void ROC::ArgReader::ReadNextNumber(float &f_val)
{
    if(!m_hasErrors && (m_currentArg <= m_argCount))
    {
        if(lua_isnumber(m_vm, m_currentArg))
        {
            float l_val = static_cast<float>(lua_tonumber(m_vm, m_currentArg));
            if(!std::isnan(l_val) && !std::isinf(l_val))
            {
                f_val = l_val;
                m_currentArg++;
            }
        }
    }
}
void ROC::ArgReader::ReadNextInteger(lua_Integer &f_val)
{
    if(!m_hasErrors && (m_currentArg <= m_argCount))
    {
        if(lua_isinteger(m_vm, m_currentArg))
        {
            f_val = lua_tointeger(m_vm, m_currentArg);
            m_currentArg++;
        }
    }
}
void ROC::ArgReader::ReadNextInteger(int &f_val)
{
    if(!m_hasErrors && (m_currentArg <= m_argCount))
    {
        if(lua_isinteger(m_vm, m_currentArg))
        {
            f_val = static_cast<int>(lua_tointeger(m_vm, m_currentArg));
            m_currentArg++;
        }
    }
}
void ROC::ArgReader::ReadNextInteger(unsigned int &f_val)
{
    if(!m_hasErrors && (m_currentArg <= m_argCount))
    {
        if(lua_isinteger(m_vm, m_currentArg))
        {
            f_val = static_cast<unsigned int>(lua_tointeger(m_vm, m_currentArg));
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
            f_val.append(lua_tostring(m_vm, m_currentArg));
            m_currentArg++;
        }
    }
}
void ROC::ArgReader::ReadNextUserdata(void **f_val, unsigned int f_type)
{
    if(!m_hasErrors && (m_currentArg <= m_argCount))
    {
        if(lua_islightuserdata(m_vm, m_currentArg))
        {
            void *a = const_cast<void*>(lua_topointer(m_vm, m_currentArg));
            if(m_core->GetMemoryManager()->CheckMemoryPointer(a, f_type))
            {
                *f_val = a;
                m_currentArg++;
            }
        }
    }
}
void ROC::ArgReader::ReadNextPointer(void **f_val)
{
    if(!m_hasErrors && (m_currentArg <= m_argCount))
    {
        if(lua_islightuserdata(m_vm, m_currentArg))
        {
            *f_val = const_cast<void*>(lua_topointer(m_vm, m_currentArg));
            m_currentArg++;
        }
    }
}

void ROC::ArgReader::ReadTableNumbers(std::vector<lua_Number> &f_vec, int f_size)
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
                        lua_Number l_val = lua_tonumber(m_vm, -1);
                        if(!std::isnan(l_val) && !std::isinf(l_val))
                        {
                            f_vec.push_back(l_val);
                            lua_pop(m_vm, 1);
                        }
                        else
                        {
                            m_error.append("Got NaN/Inf");
                            break;
                        }
                    }
                    else
                    {
                        m_error.append("Not enough table values");
                        break;
                    }
                }
                lua_insert(m_vm, m_currentArg);
                if(m_error.empty()) m_currentArg++;
                else m_hasErrors = true;
            }
            else
            {
                m_error.append("Expected table");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.append("Not enough arguments");
            m_hasErrors = true;
        }
    }
}
void ROC::ArgReader::ReadTableNumbers(std::vector<float> &f_vec, int f_size)
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
                        float l_val = static_cast<float>(lua_tonumber(m_vm, -1));
                        if(!std::isnan(l_val) && !std::isinf(l_val))
                        {
                            f_vec.push_back(l_val);
                            lua_pop(m_vm, 1);
                        }
                        else
                        {
                            m_error.append("Got NaN/Inf");
                            break;
                        }
                    }
                    else
                    {
                        m_error.append("Not enough table values");
                        break;
                    }
                }
                lua_insert(m_vm, m_currentArg);
                if(m_error.empty()) m_currentArg++;
                else m_hasErrors = true;
            }
            else
            {
                m_error.append("Expected table");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.append("Not enough arguments");
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
                        f_vec.push_back(lua_tostring(m_vm, -1));
                        lua_pop(m_vm, 1);
                    }
                    else
                    {
                        m_error.append("Not enough table values");
                        break;
                    }
                }
                lua_insert(m_vm, m_currentArg);
                if(m_error.empty()) m_currentArg++;
                else m_hasErrors = true;
            }
            else
            {
                m_error.append("Expected table");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.append("Not enough arguments");
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
    lua_pushstring(m_vm, f_val.c_str());
    m_returnValue++;
}
void ROC::ArgReader::PushPointer(void *f_val)
{
    lua_pushlightuserdata(m_vm, f_val);
    m_returnValue++;
}
void ROC::ArgReader::PushMatrix(glm::mat4 &f_val)
{
    float *l_matPointer = reinterpret_cast<float*>(&f_val);
    lua_newtable(m_vm);
    for(int i = 0; i < 16; i++)
    {
        lua_pushnumber(m_vm, i + 1);
        lua_pushnumber(m_vm, l_matPointer[i]);
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
        m_core->GetLogManager()->Log(l_log);
    }
    return m_hasErrors;
}