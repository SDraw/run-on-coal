#include "stdafx.h"

#include "Utils/LuaUtils.h"

namespace LuaUtils
{

void lua_registerClass(lua_State *f_vm, const char *f_name, lua_CFunction f_func)
{
    if(f_func) lua_register(f_vm, f_name, f_func);
    luaL_newmetatable(f_vm, f_name);
    lua_pushvalue(f_vm, -1);
    lua_setfield(f_vm, -2, "__index");
    lua_pushboolean(f_vm, 0);
    lua_setfield(f_vm, -2, "__metatable");
}
void lua_registerClassMethod(lua_State *f_vm, const char *f_name, lua_CFunction f_func)
{
    lua_pushcfunction(f_vm, f_func);
    lua_setfield(f_vm, -2, f_name);
}

}
