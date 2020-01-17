#include <string>

#include "lua.hpp"

#include "LuaArgument.h"

LuaArgument::LuaArgument()
{
    m_type = LAT_Nil;
}
LuaArgument::LuaArgument(bool f_value)
{
    m_bool = f_value;
    m_type = LAT_Boolean;
}
LuaArgument::LuaArgument(lua_Number f_value)
{
    m_number = f_value;
    m_type = LAT_Number;
}
LuaArgument::LuaArgument(lua_Integer f_value)
{
    m_integer = f_value;
    m_type = LAT_Integer;
}
LuaArgument::LuaArgument(const std::string &f_value)
{
    m_string.assign(f_value);
    m_type = LAT_String;
}
LuaArgument::LuaArgument(void *f_value)
{
    m_userdata = f_value;
    m_type = LAT_LightUserdata;
}
LuaArgument::LuaArgument(void *f_value, const std::string &f_table)
{
    m_userdata = f_value;
    m_string.assign(f_table);
    m_type = LAT_Userdata;
}
LuaArgument::~LuaArgument()
{
}
