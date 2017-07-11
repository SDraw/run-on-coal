#pragma once

namespace LuaUtils
{

void lua_registerClass(lua_State *f_vm, const char *f_name, lua_CFunction f_func);
void lua_registerClassMethod(lua_State *f_vm, const char *f_name, lua_CFunction f_func);
inline void lua_registerClassFinish(lua_State *f_vm) { lua_pop(f_vm, 1); }

}
