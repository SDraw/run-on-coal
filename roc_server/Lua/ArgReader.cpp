#include "stdafx.h"

#include "Lua/ArgReader.h"
#include "Elements/Element.h"
#include "Lua/LuaFunction.h"
#include "Utils/CustomArgument.h"
#include "Utils/CustomArguments.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager/LuaManager.h"
#include "Utils/LuaUtils.h"

ROC::ArgReader::ArgReader(lua_State *f_vm)
{
    m_vm = f_vm;
    m_argCurrent = 1;
    m_argCount = lua_gettop(m_vm);
    m_returnCount = 0;
    m_hasErrors = false;
}
ROC::ArgReader::~ArgReader()
{
}

void ROC::ArgReader::SetError(const char *f_str)
{
    m_hasErrors = true;
    m_error.assign(f_str);
}

void ROC::ArgReader::ReadBoolean(bool &f_val)
{
    if(!m_hasErrors)
    {
        if(m_argCurrent <= m_argCount)
        {
            if(lua_isboolean(m_vm, m_argCurrent)) f_val = (lua_toboolean(m_vm, m_argCurrent++) == 1);
            else SetError("Expected boolean");
        }
        else SetError("Not enough arguments");
    }
}

void ROC::ArgReader::ReadText(std::string &f_val)
{
    if(!m_hasErrors)
    {
        if(m_argCurrent <= m_argCount)
        {
            if(lua_isstring(m_vm, m_argCurrent))
            {
                size_t l_size;
                const char *l_string = lua_tolstring(m_vm, m_argCurrent++, &l_size);
                f_val.assign(l_string, l_size);
            }
            else SetError("Expected string");
        }
        else SetError("Not enough arguments");
    }
}
void ROC::ArgReader::ReadFunction(LuaFunction &f_func)
{
    if(!m_hasErrors)
    {
        if(m_argCurrent <= m_argCount)
        {
            if(lua_isfunction(m_vm, m_argCurrent))
            {
                void *l_ptr = const_cast<void*>(lua_topointer(m_vm, m_argCurrent));
                lua_settop(m_vm, m_argCurrent);
                f_func.Retrieve(m_vm, l_ptr);
                lua_insert(m_vm, m_argCurrent);
                m_argCurrent++;
            }
            else SetError("Expected function");
        }
        else SetError("Not enough arguments");
    }
}
void ROC::ArgReader::ReadArgument(CustomArgument &f_data)
{
    if(!m_hasErrors)
    {
        if(m_argCurrent <= m_argCount)
        {
            switch(lua_type(m_vm, m_argCurrent))
            {
                case LUA_TNIL:
                    f_data = CustomArgument();
                    break;
                case LUA_TBOOLEAN:
                    f_data = CustomArgument(lua_toboolean(m_vm, m_argCurrent) == 1);
                    break;
                case LUA_TNUMBER:
                    f_data = CustomArgument(lua_tonumber(m_vm, m_argCurrent));
                    break;
                case LUA_TLIGHTUSERDATA:
                    f_data = CustomArgument(lua_touserdata(m_vm, m_argCurrent));
                    break;
                case LUA_TUSERDATA:
                {
                    Element *l_element = *reinterpret_cast<Element**>(lua_touserdata(m_vm, m_argCurrent));
                    if(Core::GetCore()->GetElementManager()->IsValidElement(l_element)) f_data = CustomArgument(l_element);
                } break;
                case LUA_TSTRING:
                {
                    size_t l_len;
                    const char *l_text = lua_tolstring(m_vm, m_argCurrent, &l_len);
                    std::string l_string(l_text, l_len);
                    f_data = CustomArgument(l_string);
                } break;
                default:
                    SetError("Invalid data type");
                    break;
            }
            if(!m_hasErrors) m_argCurrent++;
        }
        else SetError("Not enough arguments");
    }
}

bool ROC::ArgReader::IsNextBoolean()
{
    bool l_result = false;
    if(!m_hasErrors && (m_argCurrent <= m_argCount)) l_result = lua_isboolean(m_vm, m_argCurrent);
    return l_result;
}
bool ROC::ArgReader::IsNextNumber()
{
    bool l_result = false;
    if(!m_hasErrors && (m_argCurrent <= m_argCount)) l_result = (lua_isnumber(m_vm, m_argCurrent) == 1);
    return l_result;
}
bool ROC::ArgReader::IsNextInteger()
{
    bool l_result = false;
    if(!m_hasErrors && (m_argCurrent <= m_argCount)) l_result = (lua_isinteger(m_vm, m_argCurrent) == 1);
    return l_result;
}
bool ROC::ArgReader::IsNextText()
{
    bool l_result = false;
    if(!m_hasErrors && (m_argCurrent <= m_argCount)) l_result = (lua_isstring(m_vm, m_argCurrent) == 1);
    return l_result;
}
bool ROC::ArgReader::IsNextFunction()
{
    bool l_result = false;
    if(!m_hasErrors && (m_argCurrent <= m_argCount)) l_result = lua_isfunction(m_vm, m_argCurrent);
    return l_result;
}
bool ROC::ArgReader::IsNextUserdata()
{
    bool l_result = false;
    if(!m_hasErrors && (m_argCurrent <= m_argCount)) l_result = (lua_isuserdata(m_vm, m_argCurrent) == 1);
    return l_result;
}

void ROC::ArgReader::ReadNextBoolean(bool &f_val)
{
    if(!m_hasErrors && (m_argCurrent <= m_argCount))
    {
        if(lua_isboolean(m_vm, m_argCurrent)) f_val = (lua_toboolean(m_vm, m_argCurrent++) == 1);
    }
}
void ROC::ArgReader::ReadNextText(std::string &f_val)
{
    if(!m_hasErrors && (m_argCurrent <= m_argCount))
    {
        if(lua_isstring(m_vm, m_argCurrent))
        {
            size_t l_size;
            const char *l_string = lua_tolstring(m_vm, m_argCurrent++, &l_size);
            f_val.assign(l_string, l_size);
        }
    }
}
void ROC::ArgReader::ReadNextFunction(LuaFunction &f_func)
{
    if(!m_hasErrors && (m_argCurrent <= m_argCount))
    {
        if(lua_isfunction(m_vm, m_argCurrent))
        {
            void *l_ptr = const_cast<void*>(lua_topointer(m_vm, m_argCurrent));
            lua_settop(m_vm, m_argCurrent);
            f_func.Retrieve(m_vm, l_ptr);
            lua_insert(m_vm, m_argCurrent);
            m_argCurrent++;
        }
    }
}

void ROC::ArgReader::PushNil()
{
    lua_pushnil(m_vm);
    m_returnCount++;
}
void ROC::ArgReader::PushBoolean(bool f_val)
{
    lua_pushboolean(m_vm, f_val);
    m_returnCount++;
}
void ROC::ArgReader::PushNumber(lua_Number f_val)
{
    lua_pushnumber(m_vm, f_val);
    m_returnCount++;
}
void ROC::ArgReader::PushInteger(lua_Integer f_val)
{
    lua_pushinteger(m_vm, f_val);
    m_returnCount++;
}
void ROC::ArgReader::PushPointer(void *f_val)
{
    lua_pushlightuserdata(m_vm, f_val);
    m_returnCount++;
}
void ROC::ArgReader::PushText(const std::string &f_val)
{
    lua_pushlstring(m_vm, f_val.data(), f_val.size());
    m_returnCount++;
}
void ROC::ArgReader::PushArgument(const CustomArgument &f_data)
{
    switch(f_data.GetType())
    {
        case CustomArgument::CAT_Nil:
            PushNil();
            break;
        case CustomArgument::CAT_Boolean:
            PushBoolean(f_data.GetBoolean());
            break;
        case CustomArgument::CAT_Integer:
            PushInteger(f_data.GetInteger());
            break;
        case CustomArgument::CAT_UInteger:
            PushInteger(f_data.GetUInteger());
            break;
        case CustomArgument::CAT_Double:
            PushNumber(f_data.GetDouble());
            break;
        case CustomArgument::CAT_Float:
            PushNumber(f_data.GetFloat());
            break;
        case CustomArgument::CAT_String:
            PushText(f_data.GetString());
            break;
        case CustomArgument::CAT_Pointer:
            PushPointer(f_data.GetPointer());
            break;
        case CustomArgument::CAT_Element:
        {
            Element *l_element = f_data.GetElement();
            LuaUtils::PushElementInMetatable(m_vm, ROC_LUA_METATABLE_USERDATA, l_element, l_element->GetElementTypeName().c_str());
        } break;
    }
    m_returnCount++;
}
void ROC::ArgReader::PushElement(Element *f_element)
{
    LuaUtils::PushElementInMetatable(m_vm, ROC_LUA_METATABLE_USERDATA, f_element, f_element->GetElementTypeName().c_str());
    m_returnCount++;
}

void ROC::ArgReader::ReadArguments(CustomArguments &f_args)
{
    if(!m_hasErrors)
    {
        for(; m_argCurrent <= m_argCount; m_argCurrent++)
        {
            switch(lua_type(m_vm, m_argCurrent))
            {
                case LUA_TBOOLEAN:
                    f_args.Push(lua_toboolean(m_vm, m_argCurrent) == 1);
                    break;
                case LUA_TNUMBER:
                    f_args.Push(lua_tonumber(m_vm, m_argCurrent));
                    break;
                case LUA_TLIGHTUSERDATA:
                    f_args.Push(lua_touserdata(m_vm, m_argCurrent));
                    break;
                case LUA_TUSERDATA:
                {
                    Element *l_element = *reinterpret_cast<Element**>(lua_touserdata(m_vm, m_argCurrent));
                    if(Core::GetCore()->GetElementManager()->IsValidElement(l_element)) f_args.Push(l_element);
                } break;
                case LUA_TSTRING:
                {
                    size_t l_len;
                    const char *l_text = lua_tolstring(m_vm, m_argCurrent, &l_len);
                    std::string l_string(l_text, l_len);
                    f_args.Push(l_string);
                } break;
            }
        }
    }
}

bool ROC::ArgReader::HasErrors()
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
        l_log.append(std::to_string(m_argCurrent));
        Core::GetCore()->GetLogManager()->Log(l_log);
    }
    return m_hasErrors;
}
