#include "stdafx.h"

#include "ArgReader.h"
#include "Lua/LuaArgument.h"
#include "Lua/LuaFunction.h"

#include "Module/LuaModule.h"
#include "Lua/LuaVM.h"
#include "Utils/LuaUtils.h"

ArgReader::ArgReader(lua_State *f_vm)
{
    m_vm = f_vm;
    m_currentArgument = 1;
    m_argumentsCount = lua_gettop(m_vm);
    m_returnCount = 0;
    m_hasErrors = false;
}
ArgReader::~ArgReader()
{
}

void ArgReader::SetError(const char *f_str)
{
    m_hasErrors = true;
    m_error.assign(f_str);
}

void ArgReader::ReadBoolean(bool &f_val)
{
    if(!m_hasErrors)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            if(lua_isboolean(m_vm, m_currentArgument))
            {
                f_val = (lua_toboolean(m_vm, m_currentArgument) == 1);
                m_currentArgument++;
            }
            else SetError("Expected boolean");
        }
        else SetError("Not enough arguments");
    }
}

void ArgReader::ReadText(std::string &f_val)
{
    if(!m_hasErrors)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            if(lua_isstring(m_vm, m_currentArgument))
            {
                size_t l_size;
                const char *l_string = lua_tolstring(m_vm, m_currentArgument, &l_size);
                f_val.assign(l_string, l_size);
                m_currentArgument++;
            }
            else SetError("Expected string");
        }
        else SetError("Not enough arguments");
    }
}
void ArgReader::ReadPointer(void *&f_val)
{
    if(!m_hasErrors)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            if(lua_isuserdata(m_vm, m_currentArgument))
            {
                f_val = const_cast<void*>(lua_topointer(m_vm, m_currentArgument));
                m_currentArgument++;
            }
            else SetError("Expected string");
        }
        else SetError("Not enough arguments");
    }
}
void ArgReader::ReadFunction(LuaFunction &f_func)
{
    if(!m_hasErrors)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            if(lua_isfunction(m_vm, m_currentArgument))
            {
                void *l_ptr = const_cast<void*>(lua_topointer(m_vm, m_currentArgument));
                lua_settop(m_vm, m_currentArgument);
                f_func.Retrieve(m_vm, l_ptr);
                lua_insert(m_vm, m_currentArgument);
                m_currentArgument++;
            }
            else SetError("Expected function");
        }
        else SetError("Not enough arguments");
    }
}
void ArgReader::ReadQuat(Quat *&f_quat)
{
    if(!m_hasErrors)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            if(lua_isuserdata(m_vm, m_currentArgument))
            {
                f_quat = *reinterpret_cast<Quat**>(luaL_checkudata(m_vm, m_currentArgument, "Quat"));
                if(f_quat != nullptr) m_currentArgument++;
                else SetError("Expected Quat");
            }
            else SetError("Expected Quat");
        }
        else SetError("Not enough arguments");
    }
}
void ArgReader::ReadArgument(LuaArgument &f_argument)
{
    if(!m_hasErrors)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            switch(lua_type(m_vm, m_currentArgument))
            {
                case LUA_TNIL:
                    f_argument = LuaArgument();
                    break;
                case LUA_TNUMBER:
                {
                    if(lua_isinteger(m_vm, m_currentArgument)) f_argument = LuaArgument(lua_tointeger(m_vm, m_currentArgument));
                    else f_argument = LuaArgument(lua_tonumber(m_vm, m_currentArgument));
                    m_currentArgument++;
                } break;
                case LUA_TBOOLEAN:
                    f_argument = LuaArgument(lua_toboolean(m_vm, m_currentArgument) == 1);
                    break;
                case LUA_TLIGHTUSERDATA:
                    f_argument = LuaArgument(lua_touserdata(m_vm, m_currentArgument));
                    break;
                case LUA_TUSERDATA:
                {
                    ROC::IElement *l_element = *reinterpret_cast<ROC::IElement**>(lua_touserdata(m_vm, m_currentArgument));
                    if(LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->IsValidIElement(l_element))
                    {
                        f_argument = LuaArgument(l_element, l_element->GetElementTypeName());
                    }
                    else f_argument = LuaArgument(lua_touserdata(m_vm, m_currentArgument));
                } break;
                default:
                    f_argument = LuaArgument();
                    break;
            }
            m_currentArgument++;
        }
        else SetError("Not enough arguments");
    }
}
void ArgReader::ReadArguments(std::vector<LuaArgument> &f_arguments)
{
    if(!m_hasErrors)
    {
        while(m_currentArgument <= m_argumentsCount)
        {
            switch(lua_type(m_vm, m_currentArgument))
            {
                case LUA_TNIL:
                    f_arguments.emplace_back();
                    break;
                case LUA_TNUMBER:
                {
                    if(lua_isinteger(m_vm, m_currentArgument)) f_arguments.emplace_back(lua_tointeger(m_vm, m_currentArgument));
                    else f_arguments.emplace_back(lua_tonumber(m_vm, m_currentArgument));
                } break;
                case LUA_TBOOLEAN:
                    f_arguments.emplace_back(lua_toboolean(m_vm, m_currentArgument) == 1);
                    break;
                case LUA_TLIGHTUSERDATA:
                    f_arguments.emplace_back(lua_touserdata(m_vm, m_currentArgument));
                    break;
                case LUA_TUSERDATA:
                {
                    ROC::IElement *l_element = *reinterpret_cast<ROC::IElement**>(lua_touserdata(m_vm, m_currentArgument));
                    if(LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->IsValidIElement(l_element))
                    {
                        f_arguments.emplace_back(l_element, l_element->GetElementTypeName());
                    }
                    else f_arguments.emplace_back(lua_touserdata(m_vm, m_currentArgument));
                } break;
                default:
                    f_arguments.emplace_back();
                    break;
            }
            m_currentArgument++;
        }
    }
}

void ArgReader::ReadNextBoolean(bool &f_val)
{
    if(!m_hasErrors && (m_currentArgument <= m_argumentsCount))
    {
        if(lua_isboolean(m_vm, m_currentArgument))
        {
            f_val = (lua_toboolean(m_vm, m_currentArgument) == 1);
            m_currentArgument++;
        }
    }
}
void ArgReader::ReadNextText(std::string &f_val)
{
    if(!m_hasErrors && (m_currentArgument <= m_argumentsCount))
    {
        if(lua_isstring(m_vm, m_currentArgument))
        {
            size_t l_size;
            const char *l_string = lua_tolstring(m_vm, m_currentArgument, &l_size);
            f_val.assign(l_string, l_size);
            m_currentArgument++;
        }
    }
}
void ArgReader::ReadNextPointer(void *&f_val)
{
    if(!m_hasErrors && (m_currentArgument <= m_argumentsCount))
    {
        if(lua_isuserdata(m_vm, m_currentArgument))
        {
            f_val = const_cast<void*>(lua_topointer(m_vm, m_currentArgument));
            m_currentArgument++;
        }
    }
}
void ArgReader::ReadNextFunction(LuaFunction &f_func)
{
    if(!m_hasErrors && (m_currentArgument <= m_argumentsCount))
    {
        if(lua_isfunction(m_vm, m_currentArgument))
        {
            void *l_ptr = const_cast<void*>(lua_topointer(m_vm, m_currentArgument));
            lua_settop(m_vm, m_currentArgument);
            f_func.Retrieve(m_vm, l_ptr);
            lua_insert(m_vm, m_currentArgument);
            m_currentArgument++;
        }
    }
}

void ArgReader::PushNil()
{
    lua_pushnil(m_vm);
    m_returnCount++;
}
void ArgReader::PushBoolean(bool f_val)
{
    lua_pushboolean(m_vm, f_val);
    m_returnCount++;
}
void ArgReader::PushNumber(lua_Number f_val)
{
    lua_pushnumber(m_vm, f_val);
    m_returnCount++;
}
void ArgReader::PushInteger(lua_Integer f_val)
{
    lua_pushinteger(m_vm, f_val);
    m_returnCount++;
}
void ArgReader::PushPointer(void *f_val)
{
    lua_pushlightuserdata(m_vm, f_val);
    m_returnCount++;
}
void ArgReader::PushText(const std::string &f_val)
{
    lua_pushlstring(m_vm, f_val.data(), f_val.size());
    m_returnCount++;
}
void ArgReader::PushElement(ROC::IElement *f_element)
{
    LuaUtils::PushElementWithMetatable(m_vm, LuaVM::ms_userdataMetatableName, f_element, f_element->GetElementTypeName().c_str());
    m_returnCount++;
}
void ArgReader::PushQuat(const Quat &f_quat)
{
    *reinterpret_cast<void**>(lua_newuserdata(m_vm, sizeof(void*))) = new Quat(f_quat);
    luaL_setmetatable(m_vm, "Quat");
    m_returnCount++;
}

bool ArgReader::HasErrors()
{
    if(m_hasErrors)
    {
        lua_Debug l_ar;
        std::string l_log("Warning at ");
        lua_getstack(m_vm, 1, &l_ar);
        lua_getinfo(m_vm, "nSl", &l_ar);
        l_log.append(l_ar.source);
        l_log.push_back(':');
        l_log.append(std::to_string(l_ar.currentline));
        l_log.append(": ");
        l_log.append(m_error);
        l_log.append(" at argument ");
        l_log.append(std::to_string(m_currentArgument));
        LuaModule::GetModule()->GetEngineCore()->GetILogManager()->Log(l_log);
    }
    return m_hasErrors;
}
