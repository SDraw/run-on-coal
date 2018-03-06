#include "stdafx.h"

#include "Utils/LuaUtils.h"

namespace LuaUtils
{

void AddClass(lua_State *f_vm, const char *f_name, lua_CFunction f_func)
{
    if(f_func) lua_register(f_vm, f_name, f_func);
    luaL_newmetatable(f_vm, f_name);
    lua_pushvalue(f_vm, -1);
    lua_setfield(f_vm, -2, "__index");
    lua_pushboolean(f_vm, 0);
    lua_setfield(f_vm, -2, "__metatable");
}
void AddClassMethod(lua_State *f_vm, const char *f_name, lua_CFunction f_func)
{
    lua_pushcfunction(f_vm, f_func);
    lua_setfield(f_vm, -2, f_name);
}
void PushElementInMetatable(lua_State *f_vm, const char *f_mt, void *f_ptr, const char *f_class)
{
    luaL_getmetatable(f_vm, f_mt);
    lua_pushlightuserdata(f_vm, f_ptr);
    lua_rawget(f_vm, -2);
    if(lua_isnil(f_vm, -1))
    {
        lua_pop(f_vm, 1);
        *reinterpret_cast<void**>(lua_newuserdata(f_vm, sizeof(void*))) = f_ptr;
        luaL_setmetatable(f_vm, f_class);
        lua_pushlightuserdata(f_vm, f_ptr);
        lua_pushvalue(f_vm, -2);
        lua_rawset(f_vm, -4);
    }
    lua_remove(f_vm, -2);
}

}
